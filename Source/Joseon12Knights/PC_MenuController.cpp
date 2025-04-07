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
	else if (!bCharacterUIShown && GS->bShowCharacterSelect)
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

void APC_MenuController::SelectVS()
{
	if (AGS_FighterState* GS = GetGS())
	{
		GS->bShowCharacterSelect = true;
	}
}


// PC_MenuController.cpp
void APC_MenuController::OnCharacterSelectConfirmed(int32 NumAI)
{
	if (UGI_GameCoreInstance* GI = GetGI())
	{
		// 최소 인원 체크 (싱글)
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
				AI.SelectedCharacterID = TEXT("RandomAI"); // 또는 프리셋
				GI->PlayerLobbyInfos.Add(AI);
			}

			if (AGM_SingleMode* GM = Cast<AGM_SingleMode>(UGameplayStatics::GetGameMode(this)))
			{
				GM->ProceedToMatch(); // ConflictZone 맵 로딩
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

