#include "MainPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Joseon12Knights/HUD_VictoryScreen.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"

AMainPlayerController::AMainPlayerController()
	: InputMappingContext(nullptr),
	MoveAction(nullptr),
	JumpAction(nullptr),
	NormalAttackAction(nullptr),
	SkillAction(nullptr),
	UltimateAction(nullptr),
	RollAction(nullptr),
	GuardAction(nullptr)
{
}

void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* SubSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (InputMappingContext)
			{
				SubSystem->AddMappingContext(InputMappingContext, 0);
			}
		}
	}

	if (IsLocalController() && CharacterInfoWidgetClass)
	{
		CharacterInfoWidget = CreateWidget<UUserWidget>(this, CharacterInfoWidgetClass);
		if (CharacterInfoWidget)
		{
			CharacterInfoWidget->AddToViewport();
		}
	}
}

void AMainPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(InputComponent))
	{

		if (ResetPressedAction)
		{
			UE_LOG(LogTemp, Warning, TEXT("ResetPressedAction 존재함, 바인딩 시도")); // ✅ 추가!
			EnhancedInput->BindAction(ResetPressedAction, ETriggerEvent::Triggered, this, &AMainPlayerController::HandleVictoryEnterKey);
		}

	}

}


void AMainPlayerController::HandleVictoryEnterKey()
{

	if (VictoryScreenWidget && VictoryScreenWidget->IsInViewport())
	{
		UE_LOG(LogTemp, Warning, TEXT("▶ [Victory] VictoryScreenWidget 확인됨 → 레벨 이동"));
		UGameplayStatics::OpenLevel(this, FName("Menus"));
	}

}



void AMainPlayerController::ClientReceiveVictory_Implementation(int32 PlayerIndex, const FString& CharName)
{
	if (!VictoryScreenClass) return;

	VictoryScreenWidget = CreateWidget<UUserWidget>(this, VictoryScreenClass);
	if (!VictoryScreenWidget) return;

	VictoryScreenWidget->AddToViewport();

	if (UHUD_VictoryScreen* TypedWidget = Cast<UHUD_VictoryScreen>(VictoryScreenWidget))
	{
		TypedWidget->SetVictoryInfo(PlayerIndex, CharName);
	}
	bShowMouseCursor = true;

	FInputModeGameAndUI InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputMode.SetHideCursorDuringCapture(false);
	SetInputMode(InputMode);

}
