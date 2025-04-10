#include "PC_MenuController.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/InputSettings.h"


void APC_MenuController::BeginPlay()
{
	Super::BeginPlay();

	if (!bPressStartUIShown && PressStartWidgetClass)
	{
		CurrentWidget = CreateWidget<UUserWidget>(this, PressStartWidgetClass);
		if (CurrentWidget)
		{
			CurrentWidget->AddToViewport();
		}
		bPressStartUIShown = true;
	}
}

void APC_MenuController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	CheckAndShowUI();
}

//void APC_MenuController::CheckAndShowUI()
//{
//	AGS_FighterState* GS = GetGS();
//	if (!GS) return;
//
//	if (!bModeUIShown && GS->bShowModeSelectUI)
//	{
//		if (ModeSelectWidgetClass)
//		{
//			if (CurrentWidget) CurrentWidget->RemoveFromParent();
//			CurrentWidget = CreateWidget<UUserWidget>(this, ModeSelectWidgetClass);
//			if (CurrentWidget) CurrentWidget->AddToViewport();
//			bModeUIShown = true;
//		}
//	}
//
//	else if (GS->bShowCharacterSelect && !bCharacterUIShown) 
//	{
//		if (CharacterSelectWidgetClass)
//		{
//			if (CurrentWidget) CurrentWidget->RemoveFromParent();
//			CurrentWidget = CreateWidget<UUserWidget>(this, CharacterSelectWidgetClass);
//			if (CurrentWidget) CurrentWidget->AddToViewport();
//			bCharacterUIShown = true; 
//		}
//	}
//}

void APC_MenuController::CheckAndShowUI()
{
	AGS_FighterState* GS = GetGS();
	if (!GS) return;

	if (!bModeUIShown && GS->bShowModeSelectUI)
	{
		// 현재 떠 있는 위젯이 MapSelect라면 메인 메뉴 UI 띄우지 말기
		if (CurrentWidget && CurrentWidget->IsA(UHUD_MapSelect::StaticClass()))
		{
			UE_LOG(LogTemp, Warning, TEXT("MapSelect가 이미 떠 있으므로 메인 메뉴 UI 생략"));
			return;
		}

		// 메인 메뉴 UI 띄움
		if (ModeSelectWidgetClass)
		{
			if (CurrentWidget) CurrentWidget->RemoveFromParent();
			CurrentWidget = CreateWidget<UUserWidget>(this, ModeSelectWidgetClass);
			if (CurrentWidget) CurrentWidget->AddToViewport();
			bModeUIShown = true;
		}
	}

	else if (GS->bShowCharacterSelect && !bCharacterUIShown)
	{
		if (CharacterSelectWidgetClass)
		{
			if (CurrentWidget) CurrentWidget->RemoveFromParent();
			CurrentWidget = CreateWidget<UUserWidget>(this, CharacterSelectWidgetClass);
			if (CurrentWidget) CurrentWidget->AddToViewport();
			bCharacterUIShown = true;
		}
	}
}


AGS_FighterState* APC_MenuController::GetGS() const
{
	return GetWorld() ? GetWorld()->GetGameState<AGS_FighterState>() : nullptr;
}

UGI_GameCoreInstance* APC_MenuController::GetGI() const
{
	return GetGameInstance<UGI_GameCoreInstance>();
}

void APC_MenuController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (InputComponent)
	{
		InputComponent->BindAction("Start", IE_Pressed, this, &APC_MenuController::OnPressStart);
		InputComponent->BindAction("Confirm", IE_Pressed, this, &APC_MenuController::OnConfirmPressed);
		InputComponent->BindAction("GameStart", IE_Pressed, this, &APC_MenuController::OnConfirmPressed);

	}
}


void APC_MenuController::SelectCharacter(const FString& CharacterID)
{
	if (AGM_BaseMode* GM = Cast<AGM_BaseMode>(UGameplayStatics::GetGameMode(this)))
	{
		GM->OnCharacterSelected(this, CharacterID);
	}
}

void APC_MenuController::OnPressStart()
{
	if (!bModeUIShown && ModeSelectWidgetClass)
	{
		if (CurrentWidget) CurrentWidget->RemoveFromParent();
		CurrentWidget = CreateWidget<UUserWidget>(this, ModeSelectWidgetClass);
		if (CurrentWidget) CurrentWidget->AddToViewport();
		bModeUIShown = true;
	}
}

void APC_MenuController::OnGameStartPressed()
{
	if (CurrentWidget)
	{
		if (UHUD_MapSelect* MapSelectUI = Cast<UHUD_MapSelect>(CurrentWidget))
		{
			MapSelectUI->ConfirmSelection(); 
		}
	}


}


void APC_MenuController::OnConfirmPressed()
{
	if (UGI_GameCoreInstance* GI = GetGI())
	{
		// 싱글 모드 && 캐릭터 선택 완료 상태일 때만 맵 셀렉트 진입
		if (GI->SelectedPlayMode == EPlayMode::Single && !GI->SelectedCharacterID.IsEmpty())
		{
			UE_LOG(LogTemp, Log, TEXT("🎮 Enter pressed - Character selected, showing Map Select"));

			// 기존 위젯 제거
			if (CurrentWidget)
			{
				CurrentWidget->RemoveFromParent();
				CurrentWidget = nullptr;
			}

			// ✅ GameState UI 상태 초기화 (꼬임 방지)
			if (AGS_FighterState* GS = GetGS())
			{
				GS->bShowModeSelectUI = false;
				GS->bShowCharacterSelect = false;
			}

			// ✅ 맵 셀렉트 UI 생성
			if (MapSelectWidgetClass)
			{
				CurrentWidget = CreateWidget<UUserWidget>(this, MapSelectWidgetClass);
				if (CurrentWidget)
				{
					CurrentWidget->AddToViewport();

					// ✅ UI 포커스 고정 (허공 클릭 방지용)
					FInputModeUIOnly InputMode;
					InputMode.SetWidgetToFocus(CurrentWidget->TakeWidget());
					InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
					SetInputMode(InputMode);
					bShowMouseCursor = true;

					UE_LOG(LogTemp, Log, TEXT("✅ MapSelect UI opened and input mode locked"));
				}

				// ✅ UI 상태 기록
				bCharacterUIShown = false;
				bModeUIShown = false;
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("❌ Enter ignored - character not selected or not in single mode"));
		}
	}
}


void APC_MenuController::SelectVS()
{
	if (UGI_GameCoreInstance* GI = GetGI())
	{
		GI->SelectedPlayMode = EPlayMode::Single; 
		GI->bIsHost = true; 
	}

	if (AGS_FighterState* GS = GetGS())
	{
		GS->bShowCharacterSelect = true; 
	}
}


void APC_MenuController::OnCharacterSelectConfirmed(int32 NumAI)
{
	if (UGI_GameCoreInstance* GI = GetGI())
	{
		if (GI->SelectedPlayMode == EPlayMode::Single)
		{
			if (NumAI < 1)
			{
				UE_LOG(LogTemp, Warning, TEXT("At least 1 AI must be selected"));
				return;
			}
			GI->PlayerLobbyInfos.Empty();

			FPlayerLobbyInfo Self;
			Self.PlayerName = GI->LocalPlayerName;
			Self.bIsReady = true;
			Self.SelectedCharacterID = GI->SelectedCharacterID;
			GI->PlayerLobbyInfos.Add(Self);

			for (int32 i = 0; i < NumAI; ++i)
			{
				FPlayerLobbyInfo AI;
				AI.PlayerName = FString::Printf(TEXT("AI_%d"), i + 1);
				AI.bIsReady = true;
				AI.SelectedCharacterID = TEXT("RandomAI");
				GI->PlayerLobbyInfos.Add(AI);
			}

			if (AGM_SingleMode* GM = Cast<AGM_SingleMode>(UGameplayStatics::GetGameMode(this)))
			{
				GM->ProceedToMatch(); 
			}
		}
		else if (GI->SelectedPlayMode == EPlayMode::Online)
		{
			if (AGM_LobbyMode* GM = Cast<AGM_LobbyMode>(UGameplayStatics::GetGameMode(this)))
			{
				GM->Server_SetReady(this);
			}
		}
	}
}

void APC_MenuController::HandleBackToCharacterSelect()
{
	if (CurrentWidget)
	{
		CurrentWidget->RemoveFromParent();
		CurrentWidget = nullptr;
	}

	if (UGI_GameCoreInstance* GI = GetGI())
	{
		UE_LOG(LogTemp, Warning, TEXT("🧠 CPU 저장된 개수: %d"), GI->CpuCharacterIDs.Num());
	}

	if (CharacterSelectWidgetClass)
	{
		CurrentWidget = CreateWidget<UUserWidget>(this, CharacterSelectWidgetClass);
		if (CurrentWidget)
		{
			CurrentWidget->AddToViewport();

			FInputModeGameAndUI InputMode;
			InputMode.SetWidgetToFocus(CurrentWidget->TakeWidget());
			InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			InputMode.SetHideCursorDuringCapture(false);
			SetInputMode(InputMode);
			bShowMouseCursor = true;
		}
	}

	bCharacterUIShown = true;
	bModeUIShown = false;
}

void APC_MenuController::HandleBackToMainMenu()
{
	if (CurrentWidget)
	{
		CurrentWidget->RemoveFromParent();
		CurrentWidget = nullptr;
	}

	if (UGI_GameCoreInstance* GI = GetGI())
	{
		GI->SelectedCharacterID = TEXT("");
		GI->SelectedCpuCount = 0;
		GI->CpuCharacterIDs.Empty();
	}

	bModeUIShown = false;
	bCharacterUIShown = false;

	if (AGS_FighterState* GS = GetGS())
	{
		GS->bShowCharacterSelect = false;
		GS->bShowModeSelectUI = true;
	}

	if (ModeSelectWidgetClass)
	{
		CurrentWidget = CreateWidget<UUserWidget>(this, ModeSelectWidgetClass);
		if (CurrentWidget)
		{
			CurrentWidget->AddToViewport();

			FInputModeGameAndUI InputMode;
			InputMode.SetWidgetToFocus(CurrentWidget->TakeWidget());
			InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			InputMode.SetHideCursorDuringCapture(false);
			SetInputMode(InputMode);
			bShowMouseCursor = true;
		}
	}
}




