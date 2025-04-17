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

	UE_LOG(LogTemp, Warning, TEXT("▶ SetupInputComponent 진입")); // ✅ 추가!

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(InputComponent))
	{
		UE_LOG(LogTemp, Warning, TEXT("▶ EnhancedInputComponent 캐스팅 성공")); // ✅ 추가!

		if (ResetPressedAction)
		{
			UE_LOG(LogTemp, Warning, TEXT("▶ ResetPressedAction 존재함, 바인딩 시도")); // ✅ 추가!
			EnhancedInput->BindAction(ResetPressedAction, ETriggerEvent::Triggered, this, &AMainPlayerController::HandleVictoryEnterKey);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("❌ ResetPressedAction 비어있음!")); // ❌ 확인
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("❌ EnhancedInputComponent 캐스팅 실패")); // ❌ 확인
	}
}


void AMainPlayerController::HandleVictoryEnterKey()
{
	UE_LOG(LogTemp, Warning, TEXT("▶ [Victory] Enter / Reset_Pressed 입력 감지됨"));

	if (VictoryScreenWidget && VictoryScreenWidget->IsInViewport())
	{
		UE_LOG(LogTemp, Warning, TEXT("▶ [Victory] VictoryScreenWidget 확인됨 → 레벨 이동"));
		UGameplayStatics::OpenLevel(this, FName("Menus"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("❌ [Victory] VictoryScreenWidget 없음 또는 비활성 상태"));
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
