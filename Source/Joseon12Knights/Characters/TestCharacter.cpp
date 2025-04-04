#include "TestCharacter.h"
#include "EnhancedInputComponent.h"

void ATestCharacter::Skill(const FInputActionValue& Value)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance && SkillAttackMontage)
	{
		AnimInstance->StopAllMontages(1);
		AnimInstance->Montage_Play(SkillAttackMontage);
	}

	if (Value.Get<bool>())
	{
		
	}
}

void ATestCharacter::Ultimate(const FInputActionValue& Value)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance && UltimateMontage)
	{
		AnimInstance->StopAllMontages(1);
		AnimInstance->Montage_Play(UltimateMontage);
	}

	if (Value.Get<bool>())
	{

	}
}
