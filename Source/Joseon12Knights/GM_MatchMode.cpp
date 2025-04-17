#include "GM_MatchMode.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerStart.h"
#include "MainPlayerController.h" 
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

	AMainPlayerState* PS = Cast<AMainPlayerState>(NewPlayer->PlayerState);
	if (PS)
	{
		PS->SetStock(3); 
		PS->SetPlayerIndex(PlayerIndex);
	}
}

int32 AGM_MatchMode::GetAlivePlayerCount() const
{
	int32 Count = 0;
	for (APlayerState* PS : GameState->PlayerArray)
	{
		AMainPlayerState* MPS = Cast<AMainPlayerState>(PS);
		if (MPS && MPS->GetStock() > 0)
		{
			++Count;
		}
	}
	return Count;
}


void AGM_MatchMode::HandlePlayerRespawn(AActor* PlayerActor)
{
	APawn* OldPawn = Cast<APawn>(PlayerActor);
	AController* Controller = OldPawn ? OldPawn->GetController() : nullptr;
	if (!Controller) return;

	AMainPlayerState* PS = Cast<AMainPlayerState>(Controller->PlayerState);
	if (!PS) return;

	int32 Lives = PS->GetStock();
	PS->SetStock(--Lives);

	if (Lives <= 0)
	{
		OldPawn->Destroy();
		UE_LOG(LogTemp, Warning, TEXT("Player %d: 스톡 소진. 리스폰 불가"), PS->GetPlayerId());

		if (GetAlivePlayerCount() == 1)
		{
			EndGame(); // 이미 작성된 함수일 경우
		}
		return;
	}

	// ===== 리스폰 처리 시작 =====

	int32 PlayerIndex = PS->GetPlayerIndex(); 
	UGI_GameCoreInstance* GI = Cast<UGI_GameCoreInstance>(GetGameInstance());
	
	if (!GI->PlayerCharacterMap.Contains(PlayerIndex))
	{
		UE_LOG(LogTemp, Error, TEXT("❌ PlayerCharacterMap에 PlayerIndex %d 없음"), PlayerIndex);
		return;
	}

	if (!GI->PlayerCharacterMap.Contains(PlayerIndex))
	{
		UE_LOG(LogTemp, Error, TEXT("❌ PlayerCharacterMap에 PlayerIndex %d 없음"), PlayerIndex);
		return;
	}

	FString CharacterKey = GI->PlayerCharacterMap[PlayerIndex];
	if (!CharacterBPMap.Contains(CharacterKey)) return;

	TSubclassOf<APawn> CharacterClass = CharacterBPMap[CharacterKey];
	if (!CharacterClass) return;

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
		UE_LOG(LogTemp, Error, TEXT("리스폰 실패: PlayerStart '%s' 없음"), *SpawnName);
		return;
	}


	if (OldPawn) OldPawn->Destroy();

	FVector SpawnLoc = SpawnPoint->GetActorLocation() + FVector(0, 0, 50);
	FRotator SpawnRot = SpawnPoint->GetActorRotation();
	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	APawn* NewPawn = GetWorld()->SpawnActor<APawn>(CharacterClass, SpawnLoc, SpawnRot, Params);
	if (NewPawn)
	{
		Controller->Possess(NewPawn);
		if (APlayerCharacter* PCChar = Cast<APlayerCharacter>(NewPawn))
		{
			PCChar->Respawn(); // 체력 초기화
		}

		UE_LOG(LogTemp, Warning, TEXT("Player %d 리스폰 완료 at %s"), PlayerIndex, *SpawnName);
	}
}



bool AGM_MatchMode::CanRespawn(AActor* PlayerActor) const
{
	APawn* Pawn = Cast<APawn>(PlayerActor);
	if (!Pawn) return false;

	AController* PC = Pawn->GetController();
	if (!PC) return false;

	AMainPlayerState* PS = Cast<AMainPlayerState>(PC->PlayerState);
	if (!PS) return false;

	return PS->GetStock() > 0;
}

void AGM_MatchMode::EndGame()
{
	UE_LOG(LogTemp, Warning, TEXT("Game SET"));

	int32 WinnerIndex = -1;
	FString WinnerCharacter;

	// 우선 승자 찾기
	for (APlayerState* PS : GameState->PlayerArray)
	{
		AMainPlayerState* MPS = Cast<AMainPlayerState>(PS);
		if (MPS && MPS->GetStock() > 0)
		{
			WinnerIndex = MPS->GetPlayerIndex();
			WinnerCharacter = MPS->GetCharacterName();
			break; // 승자 하나만 찾으면 됨
		}
	}

	// 모든 플레이어에게 승리 UI 전달
	for (APlayerState* PS : GameState->PlayerArray)
	{
		if (AMainPlayerState* MPS = Cast<AMainPlayerState>(PS))
		{
			if (AMainPlayerController* MPC = Cast<AMainPlayerController>(MPS->GetOwner()))
			{
				MPC->ClientReceiveVictory(WinnerIndex, WinnerCharacter); // 승자 정보 넘겨줌
			}
		}
	}
}


