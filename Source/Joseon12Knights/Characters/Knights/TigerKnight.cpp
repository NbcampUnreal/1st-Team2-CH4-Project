

#include "TigerKnight.h"
#include "EnhancedInputComponent.h"

void ATigerKnight::Skill(const FInputActionValue& Value)
{
	if (!bCanUseSkill) //��ų�� ��Ÿ�� �϶�
	{
		UE_LOG(LogTemp, Warning, TEXT("TigerKnight W is	CoolDown.."));
		return;
	}

	// �ִ� ��Ÿ�� ���
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && SkillAttackMontage)
	{
		AnimInstance->StopAllMontages(1);
		AnimInstance->Montage_Play(SkillAttackMontage);
	}

	// ȣ������ �г� ���� (���ݷ� , ���ݼӵ� ����)
	if (UBuffComponent* BuffComp = FindComponentByClass<UBuffComponent>())
	{
		FBuffInfo FastBuff;
		FastBuff.BuffType = EBuffType::AttackBuff;
		FastBuff.AttackMultiplier = 1.25f;  // ���ݷ� +25% ����
		FastBuff.Duration = 4.0f;
		BuffComp->AddBuff(FastBuff);
		


		
	}

	// ��ٿ� ���� (��: 5��)
	bCanUseSkill = false;  
	GetWorldTimerManager().SetTimer(SkillCooldownTimerHandle, [this]()
		{
			bCanUseSkill = true;
			UE_LOG(LogTemp, Warning, TEXT("TigerKnight W Skill is available"));
		}, 4.0f, false);

	if (TigerSkill1Effect)
	{
		UGameplayStatics::SpawnEmitterAttached(TigerSkill1Effect,     //����� ��ƼŬ ����Ʈ
			GetMesh(),   //��� ������
			FName("SkillSocket"));   //� ���Ͽ� ������

	}

	if (TigerSkill1Effect)
	{
		UGameplayStatics::PlaySoundAtLocation(this, TigerSkill1Sound, GetActorLocation());
	}

	UE_LOG(LogTemp, Warning, TEXT("AUHUNG!!"));
}

void ATigerKnight::Ultimate(const FInputActionValue& Value)
{
	if (!bCanUseUltimate)
	{
		UE_LOG(LogTemp, Warning, TEXT("TigerKnight ultimate is CoolDown..."));
		return;
	}

	// �ñر� ��Ÿ�� ���
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && UltimateMontage)
	{
		AnimInstance->StopAllMontages(1);
		AnimInstance->Montage_Play(UltimateMontage);
	}

	// ���� ȿ�� ���� (������ ���� ����, 4�� ����, ���� ���� ����)
	if (UBuffComponent* BuffComp = FindComponentByClass<UBuffComponent>())
	{
		FBuffInfo BloodBuff;
		BloodBuff.BuffType = EBuffType::Bleed;  //bleed�� ����?
		BloodBuff.Duration = 4.0f;
		BloodBuff.DamageOverTimePercent = 0.1f; // ���� ���ϸ� ��� �ұ�? 1)AI ���� ������ ? 2)ĳ���� ���ݷ� �ø���?
		BuffComp->AddBuff(BloodBuff);

		
	}

	if (TigerUltimateEffect)
	{
		UGameplayStatics::SpawnEmitterAttached(TigerUltimateEffect, GetMesh(), FName("UltimateSocket"));
	}

	if (TigerUltimateSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, TigerUltimateSound, GetActorLocation());
	}

	UE_LOG(LogTemp, Warning, TEXT("King`s Judgement!!!"));

	// ��ٿ� ���� (��: 8��)
	bCanUseUltimate = false;
	GetWorldTimerManager().SetTimer(UltimateCooldownTimerHandle, [this]()
		{
			bCanUseUltimate = true;
			UE_LOG(LogTemp, Warning, TEXT("TigerKnight Ultimate is available!"));
		}, 8.0f, false);
}
