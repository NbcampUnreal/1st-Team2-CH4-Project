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

void APC_MenuController::CheckAndShowUI()
{
	AGS_FighterState* GS = GetGS();
	if (!GS) return;

	if (!bModeUIShown && GS->bShowModeSelectUI)
	{
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

void APC_MenuController::OnConfirmPressed()
{
	if (UGI_GameCoreInstance* GI = GetGI())
	{
		if (GI->SelectedPlayMode == EPlayMode::Single && !GI->SelectedCharacterID.IsEmpty())
		{
			UE_LOG(LogTemp, Log, TEXT("Enter pressed - Character selected, showing map select"));

			if (CurrentWidget)
			{
				CurrentWidget->RemoveFromParent();
			}

			if (MapSelectWidgetClass)
			{
				CurrentWidget = CreateWidget<UUserWidget>(this, MapSelectWidgetClass);
				if (CurrentWidget)
				{
					CurrentWidget->AddToViewport();
				}
			}
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




