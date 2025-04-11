


#include "OxKnight.h"
#include "EnhancedInputComponent.h"

void AOxKnight::Skill(const FInputActionValue& Value)
{
	if(!bCanUseSkill)
	{
		UE_LOG(LogTemp, Warning, TEXT("OxKnight W is CoolDown.."));
		return;
	}

	// �ִ� ��Ÿ�� ���
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && SkillAttackMontage)
	{
		AnimInstance->StopAllMontages(1);
		AnimInstance->Montage_Play(SkillAttackMontage);
	}

	// ö�� ��ȣ(���� ���� + �̼Ӱ���)
	if (UBuffComponent* BuffComp = FindComponentByClass<UBuffComponent>())
	{

		FBuffInfo GuardianBuff;
		GuardianBuff.BuffType = EBuffType::DefenseBuff;
		GuardianBuff.Duration = 4.0f;
		GuardianBuff.MoveSpeedMultiplier = 0.7f;  //�̼Ӱ���
		GuardianBuff.DefenseMultiplier = 0.6f; //40% ���ذ���
		BuffComp->AddBuff(GuardianBuff);

		
	}

	// ��ٿ� ���� (��: 5��)
	bCanUseSkill = false;
	GetWorldTimerManager().SetTimer(SkillCooldownTimerHandle, [this]()
		{
			bCanUseSkill = true;
			UE_LOG(LogTemp, Warning, TEXT("OxKnight W Skill is available"));
		}, 5.0f, false);

	if (OXSkill1Effect)
	{
		UGameplayStatics::SpawnEmitterAttached(OXSkill1Effect, GetMesh(), FName("SkillSocket"));
	}

	if (OXSkill1Sound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, OXSkill1Sound, GetActorLocation());
	}

	UE_LOG(LogTemp, Warning, TEXT("Guard!!!"));
}

void AOxKnight::Ultimate(const FInputActionValue& Value)
{
	// �������� ������ ���ϰ� �˹�ȿ�� 
	if (!bCanUseUltimate)
	{
		UE_LOG(LogTemp, Warning, TEXT("OxKnight ultimate is CoolDown..."));
		return;
	}

	// �ñر� ��Ÿ�� ���
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && UltimateMontage)
	{
		AnimInstance->StopAllMontages(1);
		AnimInstance->Montage_Play(UltimateMontage);
	}

	//���ϰ� �˹�
	if (UBuffComponent* BuffComp = FindComponentByClass<UBuffComponent>())
	{
		FBuffInfo StunBuff;
		StunBuff.BuffType = EBuffType::DefenseBuff;
		StunBuff.Duration = 4.0f;
		BuffComp->AddBuff(StunBuff);

	}

	if (OXUltimateEffect)
	{
		UGameplayStatics::SpawnEmitterAttached(OXUltimateEffect, GetMesh(), FName("UltimateSocket"));
	}

	if (OXUltimateSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, OXUltimateSound, GetActorLocation());
	}

	UE_LOG(LogTemp, Warning, TEXT("Fierce Rush!!!!!"));

	// ��ٿ� ���� (��: 8��)
	bCanUseUltimate = false;
	GetWorldTimerManager().SetTimer(UltimateCooldownTimerHandle, [this]()
		{
			bCanUseUltimate = true;
			UE_LOG(LogTemp, Warning, TEXT("OxKnight Ultimate is available!"));
		}, 8.0f, false);
}

