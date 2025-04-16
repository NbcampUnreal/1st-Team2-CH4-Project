#include "GM_MatchMode.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerStart.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "GI_GameCoreInstance.h"
#include "PS_FighterPlayerState.h"

void AGM_MatchMode::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("🏁 GM_MatchMode BeginPlay"));

	UGI_GameCoreInstance* GI = Cast<UGI_GameCoreInstance>(GetGameInstance());
	if (!GI)
	{
		UE_LOG(LogTemp, Error, TEXT("❌ GameInstance 캐스팅 실패"));
		return;
	}

	for (const auto& Elem : GI->PlayerCharacterMap)
	{
		int32 PlayerIndex = Elem.Key;
		FString CharacterKey = Elem.Value;

		UE_LOG(LogTemp, Warning, TEXT("🎮 PlayerIndex: %d → CharacterKey: %s"), PlayerIndex, *CharacterKey);

		if (!CharacterBPMap.Contains(CharacterKey))
		{
			UE_LOG(LogTemp, Error, TEXT("❌ CharacterBPMap에 Key '%s' 없음"), *CharacterKey);
			continue;
		}

		TSubclassOf<APawn> CharacterClass = CharacterBPMap[CharacterKey];
		if (!CharacterClass)
		{
			UE_LOG(LogTemp, Error, TEXT("❌ CharacterClass is nullptr for Key '%s'"), *CharacterKey);
			continue;
		}

		FString SpawnName = (PlayerIndex == 0)
			? TEXT("Player1spawn")
			: FString::Printf(TEXT("Player1spawn_%d"), PlayerIndex - 1);

		AActor* SpawnPoint = nullptr;
		for (TActorIterator<APlayerStart> It(GetWorld()); It; ++It)
		{
			if (It->GetName().Equals(SpawnName, ESearchCase::IgnoreCase))
			{
				SpawnPoint = *It;
				break;
			}
		}

		if (!SpawnPoint)
		{
			UE_LOG(LogTemp, Error, TEXT("❌ SpawnPoint '%s' 없음"), *SpawnName);
			continue;
		}

		FVector SpawnLoc = SpawnPoint->GetActorLocation() + FVector(0, 0, 50);
		FRotator SpawnRot = SpawnPoint->GetActorRotation();
		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		APawn* SpawnedPawn = GetWorld()->SpawnActor<APawn>(CharacterClass, SpawnLoc, SpawnRot, Params);
		if (SpawnedPawn)
		{
			UE_LOG(LogTemp, Warning, TEXT("✅ Player %d: '%s' 스폰 완료 at %s"), PlayerIndex, *CharacterClass->GetName(), *SpawnName);
			SpawnedCharacters.Add(PlayerIndex, SpawnedPawn);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("❌ Player %d: Spawn 실패 → %s"), PlayerIndex, *CharacterClass->GetName());
		}
	}
}

void AGM_MatchMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	UE_LOG(LogTemp, Warning, TEXT("📥 PostLogin: %s"), *NewPlayer->GetName());

	int32 PlayerIndex = GetNumPlayers() - 1;

	if (SpawnedCharacters.Contains(PlayerIndex))
	{
		APawn* MyPawn = SpawnedCharacters[PlayerIndex];
		if (MyPawn)
		{
			NewPlayer->Possess(MyPawn);
			UE_LOG(LogTemp, Warning, TEXT("🎮 PostLogin: PlayerIndex %d Possess 성공 → %s"), PlayerIndex, *MyPawn->GetName());
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("❌ PostLogin: PlayerIndex %d용 Pawn 없음"), PlayerIndex);
	}
}
