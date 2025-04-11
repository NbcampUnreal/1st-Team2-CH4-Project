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
	UE_LOG(LogTemp, Warning, TEXT("🎬 ShowUI 호출됨 - TargetScreen: %s"), *UEnum::GetValueAsString(TargetScreen));

	if (!OwningPC)
	{
		UE_LOG(LogTemp, Error, TEXT("❌ ShowUI 실패: OwningPC null"));
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
		UE_LOG(LogTemp, Warning, TEXT("🎯 ShowUI: 캐릭터 셀렉 선택됨"));
		TargetClass = CharacterSelectClass;
		break;
	case EUIScreen::MapSelect:
		TargetClass = MapSelectClass;
		break;
	default:
		UE_LOG(LogTemp, Warning, TEXT("⚠️ ShowUI: 알 수 없는 TargetScreen"));
		return;
	}

	if (!TargetClass)
	{
		UE_LOG(LogTemp, Error, TEXT("❌ ShowUI: TargetClass가 nullptr"));
		return;
	}

	CurrentWidget = CreateWidget<UUserWidget>(OwningPC, TargetClass);
	if (!CurrentWidget)
	{
		UE_LOG(LogTemp, Error, TEXT("❌ ShowUI: CreateWidget 실패"));
		return;
	}

	CurrentWidget->AddToViewport();
	UE_LOG(LogTemp, Log, TEXT("✅ ShowUI: %s 위젯 AddToViewport 완료"), *UEnum::GetValueAsString(TargetScreen));

	FInputModeGameAndUI InputMode;
	InputMode.SetWidgetToFocus(CurrentWidget->TakeWidget());
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputMode.SetHideCursorDuringCapture(false);
	OwningPC->SetInputMode(InputMode);
	OwningPC->bShowMouseCursor = true;
}
