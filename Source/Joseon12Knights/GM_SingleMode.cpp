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
		int32 Index = 0;
		for (const FPlayerLobbyInfo& Info : Players)
		{
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
				continue;
			}

			TSubclassOf<APawn> CharacterClass = nullptr;
			if (CharacterBPMap.Contains(Info.SelectedCharacterID))
			{
				CharacterClass = CharacterBPMap[Info.SelectedCharacterID];
			}
			else
			{
				continue;
			}

			FVector Location = StartPoint->GetActorLocation();
			FRotator Rotation = StartPoint->GetActorRotation();
			APawn* SpawnedPawn = GetWorld()->SpawnActor<APawn>(CharacterClass, Location, Rotation);

			if (!SpawnedPawn)
			{
				continue;
			}

			AMainPlayerState* PS = Cast<AMainPlayerState>(SpawnedPawn->GetPlayerState());
			if (PS)
			{
				PS->SetStock(3);
			}


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
}

void AGM_SingleMode::HandlePlayerRespawn(AActor* PlayerActor)
{
	APawn* Pawn = Cast<APawn>(PlayerActor);
	if (!Pawn) return;

	AController* PC = Pawn->GetController();
	if (!PC) return;

	AMainPlayerState* PS = Cast<AMainPlayerState>(PC->PlayerState);
	if (!PS) return;

	int32 Lives = PS->GetStock();
	Lives--;
	PS->SetStock(Lives);

	if (Lives <= 0)
	{
		// 게임 종료 
	}
}

bool AGM_SingleMode::CanRespawn(AActor* PlayerActor) const
{
	APawn* Pawn = Cast<APawn>(PlayerActor);
	if (!Pawn) return false;

	AController* PC = Pawn->GetController();
	if (!PC) return false;

	AMainPlayerState* PS = Cast<AMainPlayerState>(PC->PlayerState);
	if (!PS) return false;

	return PS->GetStock() > 0;
}
