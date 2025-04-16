// GM_SingleMode.cpp

#include "GM_SingleMode.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"
#include "EngineUtils.h"
#include "GI_GameCoreInstance.h"
#include "Camera/CameraActor.h"
#include "MainPlayerController.h"

AGM_SingleMode::AGM_SingleMode()
{
	DefaultPawnClass = nullptr;
	PlayerControllerClass = AMainPlayerController::StaticClass();
}

void AGM_SingleMode::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("BeginPlay: GM_SingleMode"));

	for (TActorIterator<ACameraActor> It(GetWorld()); It; ++It)
	{
		if (It->ActorHasTag(TEXT("SingleStartCamera")))
		{
			APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
			if (PC)
			{
				PC->SetViewTargetWithBlend(*It, 1.0f);
				UE_LOG(LogTemp, Warning, TEXT("SetViewTarget → SingleStartCamera"));
			}
			break;
		}
	}

	if (UGI_GameCoreInstance* GI = GetGameInstance<UGI_GameCoreInstance>())
	{
		const TArray<FPlayerLobbyInfo>& Players = GI->PlayerLobbyInfos;
		UE_LOG(LogTemp, Warning, TEXT("PlayerLobbyInfos count: %d"), Players.Num());

		int32 Index = 0;
		for (const FPlayerLobbyInfo& Info : Players)
		{
			UE_LOG(LogTemp, Warning, TEXT("캐릭터[%d] ID: %s"), Index, *Info.SelectedCharacterID);

			FString StartTag = (Index == 0) ? TEXT("PlayerStart") : FString::Printf(TEXT("PlayerStart%d"), Index + 1);
			AActor* StartPoint = nullptr;

			for (TActorIterator<APlayerStart> It(GetWorld()); It; ++It)
			{
				if (It->ActorHasTag(FName(*StartTag)))
				{
					StartPoint = *It;
					break;
				}
			}

			if (!StartPoint)
			{
				UE_LOG(LogTemp, Error, TEXT("PlayerStart with tag '%s' not found"), *StartTag);
				continue;
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Found PlayerStart tag: %s"), *StartTag);
			}

			TSubclassOf<APawn> CharacterClass = nullptr;
			if (CharacterBPMap.Contains(Info.SelectedCharacterID))
			{
				CharacterClass = CharacterBPMap[Info.SelectedCharacterID];
				UE_LOG(LogTemp, Warning, TEXT("Character class found for ID: %s"), *Info.SelectedCharacterID);
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("CharacterBPMap에 ID '%s' 없음"), *Info.SelectedCharacterID);
				continue;
			}

			// 3. 스폰 시도
			FVector Location = StartPoint->GetActorLocation();
			FRotator Rotation = StartPoint->GetActorRotation();
			APawn* SpawnedPawn = GetWorld()->SpawnActor<APawn>(CharacterClass, Location, Rotation);

			if (!SpawnedPawn)
			{
				UE_LOG(LogTemp, Error, TEXT(" Pawn spawn 실패: ID '%s'"), *Info.SelectedCharacterID);
				continue;
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Pawn spawn 성공: %s"), *Info.SelectedCharacterID);
			}

			// 4. Possess or AI 설정
			if (Index == 0)
			{
				APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
				if (PC)
				{
					PC->Possess(SpawnedPawn);
					UE_LOG(LogTemp, Log, TEXT("Player 1 possessed 캐릭터: %s"), *Info.SelectedCharacterID);
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("GetPlayerController 실패"));
				}
			}
			else
			{
				SpawnedPawn->AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
			}

			++Index;
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("GameInstance 캐스팅 실패"));
	}
}
