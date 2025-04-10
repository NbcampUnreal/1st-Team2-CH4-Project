

#include "TigerKnight.h"
#include "EnhancedInputComponent.h"

void ATigerKnight::Skill(const FInputActionValue& Value)
{
	if (!bCanUseSkill) //스킬이 쿨타임 일때
	{
		UE_LOG(LogTemp, Warning, TEXT("TigerKnight W is	CoolDown.."));
		return;
	}

	// 애님 몽타주 재생
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && SkillAttackMontage)
	{
		AnimInstance->StopAllMontages(1);
		AnimInstance->Montage_Play(SkillAttackMontage);
	}

	// 호랑이의 분노 적용 (공격력 , 공격속도 증가)
	if (UBuffComponent* BuffComp = FindComponentByClass<UBuffComponent>())
	{
		FBuffInfo FastBuff;
		FastBuff.BuffType = EBuffType::AttackBuff;
		FastBuff.AttackMultiplier = 1.25f;  // 공격력 +25% 증가
		FastBuff.Duration = 4.0f;
		BuffComp->AddBuff(FastBuff);
		


		
	}

	// 쿨다운 시작 (예: 5초)
	bCanUseSkill = false;  
	GetWorldTimerManager().SetTimer(SkillCooldownTimerHandle, [this]()
		{
			bCanUseSkill = true;
			UE_LOG(LogTemp, Warning, TEXT("TigerKnight W Skill is available"));
		}, 4.0f, false);

	if (TigerSkill1Effect)
	{
		UGameplayStatics::SpawnEmitterAttached(TigerSkill1Effect,     //재생할 파티클 이펙트
			GetMesh(),   //어디에 붙일지
			FName("SkillSocket"));   //어떤 소켓에 붙일지

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

	// 궁극기 몽타주 재생
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && UltimateMontage)
	{
		AnimInstance->StopAllMontages(1);
		AnimInstance->Montage_Play(UltimateMontage);
	}

	// 출혈 효과 적용 (강력한 베기 공격, 4초 지속, 적의 방어력 저하)
	if (UBuffComponent* BuffComp = FindComponentByClass<UBuffComponent>())
	{
		FBuffInfo BloodBuff;
		BloodBuff.BuffType = EBuffType::Bleed;  //bleed가 출혈?
		BloodBuff.Duration = 4.0f;
		BloodBuff.DamageOverTimePercent = 0.1f; // 방어력 저하를 어떻게 할까? 1)AI 방어력 내리기 ? 2)캐락터 공격력 늘리기?
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

	// 쿨다운 시작 (예: 8초)
	bCanUseUltimate = false;
	GetWorldTimerManager().SetTimer(UltimateCooldownTimerHandle, [this]()
		{
			bCanUseUltimate = true;
			UE_LOG(LogTemp, Warning, TEXT("TigerKnight Ultimate is available!"));
		}, 8.0f, false);
}
