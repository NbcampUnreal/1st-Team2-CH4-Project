#include "GM_StoryMode.h"
#include "Kismet/GameplayStatics.h"
#include "GI_GameCoreInstance.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/PlayerStart.h"
#include "EngineUtils.h"
#include "MainPlayerState.h"
#include "MainPlayerController.h"
#include "PlayerCharacter.h"

AGM_StoryMode::AGM_StoryMode()
{
	DefaultPawnClass = nullptr;
	PlayerControllerClass = AMainPlayerController::StaticClass();
}

void AGM_StoryMode::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("🎮 Player 스폰 시도"));

	if (!StoryMapAsset.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("❌ StoryMapAsset is not set!"));
		return;
	}

	if (UGI_GameCoreInstance* GI = GetGameInstance<UGI_GameCoreInstance>())
	{
		FString PlayerCharID = GI->SelectedCharacterID;
		FString AICharID = TEXT("1"); // AI는 고정 or 이후 랜덤도 가능

		TSubclassOf<APawn> PlayerClass = CharacterBPMap.Contains(PlayerCharID) ? CharacterBPMap[PlayerCharID] : nullptr;
		TSubclassOf<APawn> AIClass = CharacterBPMap_AI.Contains(AICharID) ? CharacterBPMap_AI[AICharID] : nullptr;

		if (!PlayerClass || !AIClass)
		{
			UE_LOG(LogTemp, Error, TEXT("캐릭터 클래스가 없습니다. Player=%s AI=%s"), *PlayerCharID, *AICharID);
			return;
		}

		AActor* PlayerStart = nullptr;
		AActor* AIStart = nullptr;

		for (TActorIterator<APlayerStart> It(GetWorld()); It; ++It)
		{
			if (It->ActorHasTag(TEXT("PlayerStart")))
			{
				PlayerStart = *It;
			}
			else if (It->ActorHasTag(TEXT("PlayerStart2")))
			{
				AIStart = *It;
			}
		}

		if (!PlayerStart || !AIStart)
		{
			UE_LOG(LogTemp, Error, TEXT("❌ PlayerStart 또는 PlayerStart2가 없습니다."));
			return;
		}

		// 플레이어 스폰
		if (APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0))
		{
			APawn* PlayerPawn = GetWorld()->SpawnActor<APawn>(PlayerClass, PlayerStart->GetActorLocation(), PlayerStart->GetActorRotation());
			if (PlayerPawn)
			{
				PC->Possess(PlayerPawn);

				if (AMainPlayerState* PS = Cast<AMainPlayerState>(PlayerPawn->GetPlayerState()))
				{
					PS->SetPlayerIndex(0);
					PS->SetStock(3);
				}
			}
		}

		// AI 스폰
		APawn* AIPawn = GetWorld()->SpawnActor<APawn>(AIClass, AIStart->GetActorLocation(), AIStart->GetActorRotation());
		if (AIPawn)
		{
			AIPawn->AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

			if (AMainPlayerState* PS = Cast<AMainPlayerState>(AIPawn->GetPlayerState()))
			{
				PS->SetPlayerIndex(1);
			}
		}
	}
}

void AGM_StoryMode::HandlePlayerRespawn(AActor* PlayerActor)
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
			TriggerStoryGameEnd(false);
			return;
		}

		// 리스폰
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

		if (UGI_GameCoreInstance* GI = GetGameInstance<UGI_GameCoreInstance>())
		{
			TSubclassOf<APawn> PlayerClass = CharacterBPMap[GI->SelectedCharacterID];
			APawn* NewPawn = GetWorld()->SpawnActor<APawn>(PlayerClass, StartPoint->GetActorLocation(), StartPoint->GetActorRotation());

			if (NewPawn)
			{
				if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0))
				{
					PlayerController->Possess(NewPawn);
				}

				if (APlayerCharacter* PCChar = Cast<APlayerCharacter>(NewPawn))
				{
					PCChar->Respawn();
				}
			}

		}
	}
	else
	{
		// AI 사망 → 승리
		Pawn->Destroy();
		DeadAI++;
		if (DeadAI >= TotalAI)
		{
			TriggerStoryGameEnd(true);
		}
	}
}

bool AGM_StoryMode::CanRespawn(AActor* PlayerActor) const
{
	APawn* Pawn = Cast<APawn>(PlayerActor);
	if (!Pawn) return false;

	AController* PC = Pawn->GetController();
	if (!PC) return false;

	AMainPlayerState* PS = Cast<AMainPlayerState>(PC->PlayerState);
	if (!PS) return false;

	return PS->GetStock() > 0;
}

void AGM_StoryMode::TriggerStoryGameEnd(bool bPlayerWin)
{
	FString WinnerChar = TEXT("스토리캐릭터");

	for (APlayerState* PS : GameState->PlayerArray)
	{
		if (AMainPlayerState* MPS = Cast<AMainPlayerState>(PS))
		{
			if (MPS->GetPlayerIndex() == 0)
			{
				if (AMainPlayerController* MPC = Cast<AMainPlayerController>(MPS->GetOwner()))
				{
					int32 WinnerIndex = bPlayerWin ? 0 : 1;
					MPC->ClientReceiveVictory(WinnerIndex, WinnerChar);
				}
				break;
			}
		}
	}
}
