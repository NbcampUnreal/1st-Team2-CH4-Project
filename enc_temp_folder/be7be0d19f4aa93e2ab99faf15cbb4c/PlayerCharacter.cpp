#include "PlayerCharacter.h"
#include "EnhancedInputComponent.h"
#include "MainPlayerController.h"


// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	MoveSpeed = 50.f;
	DashDistance = 2000.f;
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (AMainPlayerController* PlayerController = Cast<AMainPlayerController>(GetController()))
		{
			if (PlayerController->MoveAction)
			{
				EnhancedInput->BindAction(
					PlayerController->MoveAction,
					ETriggerEvent::Triggered,
					this,
					&APlayerCharacter::Move
				);
			}

			if (PlayerController->JumpAction)
			{
				EnhancedInput->BindAction(
					PlayerController->JumpAction,
					ETriggerEvent::Triggered,
					this,
					&APlayerCharacter::StartJump
				);
			}

			if (PlayerController->NormalAttackAction)
			{
				EnhancedInput->BindAction(
					PlayerController->NormalAttackAction,
					ETriggerEvent::Started,
					this,
					&APlayerCharacter::NormalAttack
				);
			}

			if (PlayerController->RollAction)
			{
				EnhancedInput->BindAction(
					PlayerController->RollAction,
					ETriggerEvent::Started,
					this,
					&APlayerCharacter::Roll
				);
			}

			if (PlayerController->SkillAction)
			{
				EnhancedInput->BindAction(
					PlayerController->SkillAction,
					ETriggerEvent::Started,
					this,
					&APlayerCharacter::Skill
				);
			}

			if (PlayerController->UltimateAction)
			{
				EnhancedInput->BindAction(
					PlayerController->UltimateAction,
					ETriggerEvent::Started,
					this,
					&APlayerCharacter::Ultimate
				);
			}
		}
	}
}

void APlayerCharacter::Move(const FInputActionValue& Value)
{
	float AxisValue = Value.Get<float>();

	AddMovementInput(GetActorForwardVector(), AxisValue * MoveSpeed);
	
	float Direction = AxisValue < 0 ? 1 : -1;
	FRotator CurrentRotation = FRotator(0.f, Direction * 90.f, 0.f);
	GetMesh()->SetWorldRotation(CurrentRotation);
}

void APlayerCharacter::StartJump(const FInputActionValue& Value)
{
	if (Value.Get<bool>())
	{
		Jump();
	}
}

void APlayerCharacter::StopJump(const FInputActionValue& Value)
{
	if (!Value.Get<bool>())
	{
		StopJumping();
	}
}

void APlayerCharacter::Roll(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("ROLL"));
	
	FVector Direction = GetMesh()->GetRightVector() * DashDistance;
	LaunchCharacter(Direction, true, false);
}




