#include "TestCharacter.h"

void ATestCharacter::NormalAttack(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("Normal Attack"));
}

void ATestCharacter::Skill(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("Skill Attack"));
}

void ATestCharacter::Ultimate(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("Ultimate"));
}
