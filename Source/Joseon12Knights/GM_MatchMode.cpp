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

	UGI_GameCoreInstance* GI = Cast<UGI_GameCoreInstance>(GetGameInstance());
	if (!GI)
	{
		return;
	}

	for (const auto& Elem : GI->PlayerCharacterMap)
	{
		int32 PlayerIndex = Elem.Key;
		FString CharacterKey = Elem.Value;


		if (!CharacterBPMap.Contains(CharacterKey))
		{
			continue;
		}

		TSubclassOf<APawn> CharacterClass = CharacterBPMap[CharacterKey];
		if (!CharacterClass)
		{
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
			UE_LOG(LogTemp, Error, TEXT("SpawnPoint '%s' 없음"), *SpawnName);
			continue;
		}

		FVector SpawnLoc = SpawnPoint->GetActorLocation() + FVector(0, 0, 50);
		FRotator SpawnRot = SpawnPoint->GetActorRotation();
		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		APawn* SpawnedPawn = GetWorld()->SpawnActor<APawn>(CharacterClass, SpawnLoc, SpawnRot, Params);
		if (SpawnedPawn)
		{
			UE_LOG(LogTemp, Warning, TEXT("Player %d: '%s' 스폰 완료 at %s"), PlayerIndex, *CharacterClass->GetName(), *SpawnName);
			SpawnedCharacters.Add(PlayerIndex, SpawnedPawn);
		}

	}

	if (APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0))
	{
		int32 PlayerIndex = 0;
		if (SpawnedCharacters.Contains(PlayerIndex))
		{
			APawn* MyPawn = SpawnedCharacters[PlayerIndex];
			if (MyPawn)
			{
				PC->Possess(MyPawn);
			}

		}
	}

}

void AGM_MatchMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	int32 PlayerIndex = GetNumPlayers() - 1;

	if (SpawnedCharacters.Contains(PlayerIndex))
	{
		APawn* MyPawn = SpawnedCharacters[PlayerIndex];
		if (MyPawn)
		{
			NewPlayer->Possess(MyPawn);
		}
	}

}
