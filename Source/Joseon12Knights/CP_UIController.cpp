#include "CP_UIController.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"


UCP_UIController::UCP_UIController()
{
}
void UCP_UIController::Initialize(APlayerController* InOwner)
{
	OwningPC = InOwner;
}

void UCP_UIController::ShowUI(EUIScreen TargetScreen)
{

	if (!OwningPC)
	{
		return;
	}

	if (CurrentWidget)
	{
		CurrentWidget->RemoveFromParent();
		CurrentWidget = nullptr;
	}

	TSubclassOf<UUserWidget> TargetClass = nullptr;

	switch (TargetScreen)
	{
	case EUIScreen::PressStart:
		TargetClass = PressStartClass;
		break;
	case EUIScreen::MainMenu:
		TargetClass = MainMenuClass;
		break;
	case EUIScreen::CharacterSelect:
		TargetClass = CharacterSelectClass;
		break;
	case EUIScreen::MapSelect:
		TargetClass = MapSelectClass;
		break;
	case EUIScreen::CharacterStory:
		TargetClass = CharacterStoryClass;
		break;
	case EUIScreen::Online:         
		TargetClass = OnlineClass;
		break;
	default:
		return;
	}

	if (!TargetClass)
	{
		return;
	}

	CurrentWidget = CreateWidget<UUserWidget>(OwningPC, TargetClass);
	if (!CurrentWidget)
	{
		return;
	}

	CurrentWidget->AddToViewport();
	FInputModeGameAndUI InputMode;
	InputMode.SetWidgetToFocus(CurrentWidget->TakeWidget());
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputMode.SetHideCursorDuringCapture(false);
	OwningPC->SetInputMode(InputMode);
	OwningPC->bShowMouseCursor = true;
}
