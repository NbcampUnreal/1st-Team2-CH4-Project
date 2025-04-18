#include "GM_SingleMode.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"
#include "EngineUtils.h"
#include "GI_GameCoreInstance.h"
#include "Camera/CameraActor.h"
#include "MainPlayerController.h"
#include "MainPlayerState.h"
#include "PlayerCharacter.h"

AGM_SingleMode::AGM_SingleMode()
{
	DefaultPawnClass = nullptr;
	PlayerControllerClass = AMainPlayerController::StaticClass();
}

void AGM_SingleMode::BeginPlay()
{
	Super::BeginPlay();

	// 시작 카메라 설정
	for (TActorIterator<ACameraActor> It(GetWorld()); It; ++It)
	{
		if (It->ActorHasTag(TEXT("SingleStartCamera")))
		{
			if (APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0))
			{
				PC->SetViewTargetWithBlend(*It, 1.0f);
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
			if (!StartPoint) continue;

			FString CharacterID = Info.SelectedCharacterID;
			TSubclassOf<APawn> CharacterClass = (Index == 0)
				? CharacterBPMap[CharacterID]
				: CharacterBPMap_AI[CharacterID];

				if (!CharacterClass)
				{
					UE_LOG(LogTemp, Warning, TEXT("❌ CharacterClass 없음: Index=%d, ID=%s"), Index, *CharacterID);
					continue;
				}

				FVector Location = StartPoint->GetActorLocation();
				FRotator Rotation = StartPoint->GetActorRotation();

				APawn* SpawnedPawn = GetWorld()->SpawnActor<APawn>(CharacterClass, Location, Rotation);
				if (!SpawnedPawn) continue;

				if (Index == 0)
				{
					// 플레이어 조작
					if (APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0))
					{
						PC->Possess(SpawnedPawn);
					}
				}
				else
				{
					// AI 조작
					SpawnedPawn->AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
					++TotalAI;
				}

				if (AMainPlayerState* PS = Cast<AMainPlayerState>(SpawnedPawn->GetPlayerState()))
				{
					PS->SetPlayerIndex(Index);
					if (Index == 0)
					{
						PS->SetStock(3); // 플레이어만 스톡 설정
					}
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
	AMainPlayerState* PS = PC ? Cast<AMainPlayerState>(PC->PlayerState) : Cast<AMainPlayerState>(Pawn->GetPlayerState());
	if (!PS) return;

	int32 Index = PS->GetPlayerIndex();

	if (Index == 0)
	{
		int32 Lives = PS->GetStock();
		PS->SetStock(--Lives);

		if (Lives <= 0)
		{
			Pawn->Destroy();
			TriggerSingleGameEnd(false); // 패배
			return;
		}

		// 플레이어 리스폰
		FString StartTag = TEXT("PlayerStart");
		AActor* StartPoint = nullptr;

		for (TActorIterator<APlayerStart> It(GetWorld()); It; ++It)
		{
			if (It->ActorHasTag(FName(*StartTag)))
			{
				StartPoint = *It;
				break;
			}
		}
		if (!StartPoint) return;

		UGI_GameCoreInstance* GI = Cast<UGI_GameCoreInstance>(GetGameInstance());
		if (!GI) return;

		FString CharacterID = GI->PlayerLobbyInfos[0].SelectedCharacterID;
		TSubclassOf<APawn> CharacterClass = CharacterBPMap[CharacterID];
		FVector Location = StartPoint->GetActorLocation();
		FRotator Rotation = StartPoint->GetActorRotation();

		FTimerHandle Timer;

		GetWorld()->GetTimerManager().SetTimer(
			Timer,
			FTimerDelegate::CreateLambda([Pawn,Location]()
			{
				if (Pawn)
				{
					Pawn->SetActorLocation(Location);
					if (APlayerCharacter* Player = Cast<APlayerCharacter>(Pawn))
					{
						UE_LOG(LogTemp, Warning, TEXT("Respawn Call"));
						Player->Respawn();
					}
				}
			}),
			1.0f,
			false
		);

	

	/*	APawn* NewPawn = GetWorld()->SpawnActor<APawn>(CharacterClass, Location, Rotation);
		if (!NewPawn) return;

		if (APlayerController* PlayerPC = UGameplayStatics::GetPlayerController(this, 0))
		{
			PlayerPC->Possess(NewPawn);
		}*/

		//if (APlayerCharacter* PCChar = Cast<APlayerCharacter>(NewPawn))
		//{
		//	PCChar->Respawn();
		//}
	}
	else
	{
		Pawn->Destroy();
		++DeadAI;

		if (DeadAI >= TotalAI)
		{
			TriggerSingleGameEnd(true); 
		}
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

void AGM_SingleMode::TriggerSingleGameEnd(bool bPlayerWin)
{
	FString WinnerCharacter = TEXT("None");

	for (APlayerState* PS : GameState->PlayerArray)
	{
		if (AMainPlayerState* MPS = Cast<AMainPlayerState>(PS))
		{
			if (MPS->GetPlayerIndex() == 0)
			{
				WinnerCharacter = MPS->GetCharacterName();

				if (AMainPlayerController* MPC = Cast<AMainPlayerController>(MPS->GetOwner()))
				{
					int32 WinnerIndex = bPlayerWin ? 0 : 1;
					MPC->ClientReceiveVictory(WinnerIndex, WinnerCharacter);
				}
				break;
			}
		}
	}
}
