#include "MainPlayerController.h"
#include "EnhancedInputSubsystems.h"

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
}




