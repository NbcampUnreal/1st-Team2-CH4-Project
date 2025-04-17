#include "TestCharacter.h"
#include "EnhancedInputComponent.h"

void ATestCharacter::Skill(const FInputActionValue& Value)
{
	
}

void ATestCharacter::ServerSkill_Implementation()
{

}

void ATestCharacter::MulticastSkill_Implementation()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance && SkillAttackMontage)
	{
		AnimInstance->StopAllMontages(1);
		AnimInstance->Montage_Play(SkillAttackMontage);
	}
}

void ATestCharacter::Ultimate(const FInputActionValue& Value)
{

}
void ATestCharacter::ServerUltimate_Implementation()
{

}

void ATestCharacter::MulticastUltimate_Implementation()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance && UltimateMontage)
	{
		AnimInstance->StopAllMontages(1);
		AnimInstance->Montage_Play(UltimateMontage);
	}
}
