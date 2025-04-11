#include "PC_MenuController.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

void APC_MenuController::BeginPlay()
{
	Super::BeginPlay();
	UIController = NewObject<UCP_UIController>(this, UIControllerClass);
	if (UIController)
	{
		UIController->Initialize(this);
		UIController->ShowUI(EUIScreen::PressStart);
	}

}

void APC_MenuController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void APC_MenuController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (InputComponent)
	{
		InputComponent->BindAction("GameStart", IE_Pressed, this, &APC_MenuController::OnGameStartPressed);
		InputComponent->BindAction("PressStart", IE_Pressed, this, &APC_MenuController::OnPressStartPressed);
		InputComponent->BindAction("GameEnter", IE_Pressed, this, &APC_MenuController::OnCharacterSelectEnterPressed);
		InputComponent->BindAction("StoryEnter", IE_Pressed, this, &APC_MenuController::OnCharacterStoryEnterPressed);
	}
}

void APC_MenuController::OnGameStartPressed()
{
	if (UIController && UIController->GetCurrentWidget())
	{
		if (UHUD_MapSelect* MapSelectUI = Cast<UHUD_MapSelect>(UIController->GetCurrentWidget()))
		{
			MapSelectUI->ConfirmSelection();
		}
	}
}

void APC_MenuController::OnPressStartPressed()
{
	if (UIController && UIController->GetCurrentWidget())
	{
		if (UHUD_PressStart* PressStartUI = Cast<UHUD_PressStart>(UIController->GetCurrentWidget()))
		{
			PressStartUI->SimulateStartPress();
		}
	}
}


void APC_MenuController::OnCharacterSelectEnterPressed()
{
	if (!UIController || !UIController->GetCurrentWidget()) return;

	if (UHUD_CharacterSelect* CharSelectUI = Cast<UHUD_CharacterSelect>(UIController->GetCurrentWidget()))
	{
		if (UGI_GameCoreInstance* GI = GetGI())
		{
			const bool bCharacterSelected = !GI->SelectedCharacterID.IsEmpty();
			const bool bHasAI = GI->SelectedCpuCount >= 1;

			if (bCharacterSelected && bHasAI)
			{
				UIController->ShowUI(EUIScreen::MapSelect);
			}
		}
	}
}

void APC_MenuController::OnCharacterStoryEnterPressed()
{
	if (!UIController || !UIController->GetCurrentWidget()) return;

	if (UHUD_CharacterStory* StoryUI = Cast<UHUD_CharacterStory>(UIController->GetCurrentWidget()))
	{
		StoryUI->ConfirmSelection();
	}
}


void APC_MenuController::SelectCharacter(const FString& CharacterID)
{
	if (AGM_BaseMode* GM = Cast<AGM_BaseMode>(UGameplayStatics::GetGameMode(this)))
	{
		GM->OnCharacterSelected(this, CharacterID);
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
		GS->bShowModeSelectUI = false;
	}

	if (UIController)
	{
		UIController->ShowUI(EUIScreen::CharacterSelect);
	}
}

void APC_MenuController::SelectArcade()
{
	if (UGI_GameCoreInstance* GI = GetGI())
	{
		GI->SelectedPlayMode = EPlayMode::Story;
		GI->bIsHost = true;
	}

	if (AGS_FighterState* GS = GetGS())
	{
		GS->bShowModeSelectUI = false;
	}

	if (UIController)
	{
		UIController->ShowUI(EUIScreen::CharacterStory);
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
	if (UIController)
	{
		UIController->ShowUI(EUIScreen::CharacterSelect);
	}
}

void APC_MenuController::HandleBackToMainMenu()
{
	if (UGI_GameCoreInstance* GI = GetGI())
	{
		GI->SelectedCharacterID = TEXT("");
		GI->SelectedCpuCount = 0;
		GI->CpuCharacterIDs.Empty();
	}

	if (AGS_FighterState* GS = GetGS())
	{
		GS->bShowCharacterSelect = false;
		GS->bShowModeSelectUI = true;
	}

	if (UIController)
	{
		UIController->ShowUI(EUIScreen::MainMenu);
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
