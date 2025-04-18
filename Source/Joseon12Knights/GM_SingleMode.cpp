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
	PrimaryActorTick.bCanEverTick = true;
}

void AGM_SingleMode::BeginPlay()
{
	Super::BeginPlay();
	bGameStarted = false;

	UE_LOG(LogTemp, Warning, TEXT("🎮 BeginPlay: SingleMode 시작"));

	// 카메라 설정
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
		// Player
		FString PlayerCharID = GI->SelectedCharacterID;
		SpawnCharacterAtTag(TEXT("PlayerStart"), PlayerCharID, true, 0);

		// AI
		const TArray<FString>& CpuIDs = GI->CpuCharacterIDs;
		TotalAI = CpuIDs.Num();

		for (int32 i = 0; i < CpuIDs.Num(); ++i)
		{
			FString StartTag = FString::Printf(TEXT("PlayerStart%d"), i + 2);
			SpawnCharacterAtTag(StartTag, CpuIDs[i], false, i + 1);
		}
	}

	bGameStarted = true;
}

void AGM_SingleMode::SpawnCharacterAtTag(const FString& Tag, const FString& CharID, bool bIsPlayer, int32 Index)
{
	AActor* StartPoint = nullptr;

	for (TActorIterator<APlayerStart> It(GetWorld()); It; ++It)
	{
		if (It->ActorHasTag(FName(*Tag)))
		{
			StartPoint = *It;
			break;
		}
	}
	if (!StartPoint)
	{
		return;
	}

	TSubclassOf<APawn> CharClass = bIsPlayer ? CharacterBPMap[CharID] : CharacterBPMap_AI[CharID];
	if (!CharClass)
	{
		return;
	}

	APawn* Spawned = GetWorld()->SpawnActor<APawn>(CharClass, StartPoint->GetActorLocation(), StartPoint->GetActorRotation());
	if (!Spawned)
	{
		return;
	}

	if (bIsPlayer)
	{
		if (APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0))
		{
			PC->Possess(Spawned);
		}
	}
	else
	{
		Spawned->AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

		APlayerState* NewPS = GetWorld()->SpawnActor<AMainPlayerState>(AMainPlayerState::StaticClass());
		if (NewPS)
		{
			Spawned->SetPlayerState(NewPS);
		}
	}

	if (AMainPlayerState* PS = Cast<AMainPlayerState>(Spawned->GetPlayerState()))
	{
		PS->SetPlayerIndex(Index);
		if (bIsPlayer)
		{
			PS->SetStock(3);
		}
	}

}

void AGM_SingleMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (!bGameStarted || bGameEnded || TotalAI <= 0) return;

	int32 AliveAI = 0;

	for (TActorIterator<APlayerCharacter> It(GetWorld()); It; ++It)
	{
		APlayerCharacter* AI = *It;
		if (!AI)
		{
			continue;
		}

		AMainPlayerState* PS = Cast<AMainPlayerState>(AI->GetPlayerState());
		if (!PS)
		{
			continue;
		}


		if (PS->GetPlayerIndex() != 0 && AI->IsAlive())
		{
			AliveAI++;
		}
	}


	if (AliveAI == 0)
	{
		TriggerSingleGameEnd(true);
		bGameEnded = true;
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
			TriggerSingleGameEnd(false);
			return;
		}

		AActor* StartPoint = nullptr;
		for (TActorIterator<APlayerStart> It(GetWorld()); It; ++It)
		{
			if (It->ActorHasTag(TEXT("PlayerStart")))
			{
				StartPoint = *It;
				break;
			}
		}
		if (!StartPoint) return;

		FVector Location = StartPoint->GetActorLocation();

		GetWorld()->GetTimerManager().SetTimerForNextTick([Pawn, Location]() {
			Pawn->SetActorLocation(Location);
			if (APlayerCharacter* Player = Cast<APlayerCharacter>(Pawn))
			{
				Player->Respawn();
			}
		});
	}
	else
	{
		Pawn->Destroy();
		++DeadAI;
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
