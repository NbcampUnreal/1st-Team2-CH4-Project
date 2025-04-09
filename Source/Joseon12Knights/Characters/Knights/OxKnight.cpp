


#include "OxKnight.h"
#include "EnhancedInputComponent.h"

void AOxKnight::Skill(const FInputActionValue& Value)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance && SkillAttackMontage)
	{
		AnimInstance->StopAllMontages(1);
		AnimInstance->Montage_Play(SkillAttackMontage);
	}
}

void AOxKnight::Ultimate(const FInputActionValue& Value)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance && UltimateMontage)
	{
		AnimInstance->StopAllMontages(1);
		AnimInstance->Montage_Play(UltimateMontage);
	}
}

