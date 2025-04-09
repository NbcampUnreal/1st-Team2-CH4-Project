


#include "RatKnight.h"
#include "EnhancedInputComponent.h"


void ARatKnight::Skill(const FInputActionValue& Value)
{
	// W키 스킬 쥐구멍 >> 공격형 : 첫 타격 추가 피해(1.8배) + 디버프 : 슬로우 (-30% 이속) 2초

	if (!bCanUseSkill)
	{
		UE_LOG(LogTemp, Warning, TEXT("RatKnight W Skill 쿨타임 중..."));
		return;
	}

	// 애님 몽타주 재생
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && SkillAttackMontage)
	{
		AnimInstance->StopAllMontages(1);
		AnimInstance->Montage_Play(SkillAttackMontage);
	}

	// 슬로우 버프 적용 (이동속도 70%로 2초간 유지)
	if (UBuffComponent* BuffComp = FindComponentByClass<UBuffComponent>())
	{
		FBuffInfo SlowBuff;
		SlowBuff.BuffType = EBuffType::Slow;
		SlowBuff.Duration = 2.0f;
		SlowBuff.MoveSpeedMultiplier = 0.7f; // -30% 속도
		BuffComp->AddBuff(SlowBuff);
	}

	// 쿨다운 시작 (예: 5초)
	bCanUseSkill = false;
	GetWorldTimerManager().SetTimer(SkillCooldownTimerHandle, [this]()
		{
			bCanUseSkill = true;
			UE_LOG(LogTemp, Warning, TEXT("RatKnight W 스킬 사용 가능!"));
		}, 5.0f, false);

	if (RatSkill1Effect)
	{
		UGameplayStatics::SpawnEmitterAttached(RatSkill1Effect, GetMesh(), FName("SkillSocket"));
	}

	if (RatSkill1Sound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, RatSkill1Sound, GetActorLocation());
	}

	UE_LOG(LogTemp, Warning, TEXT("쥐구멍!!!"));
}

void ARatKnight::Ultimate(const FInputActionValue& Value)
{
	// R키 궁극기 1등 난타 >> 공격형, 디버프, 1.2배 (타격당), 독 데미지 (초당 5%% 추가 피해) 4초
	if (!bCanUseUltimate)
	{
		UE_LOG(LogTemp, Warning, TEXT("RatKnight 궁극기 쿨타임 중..."));
		return;
	}

	// 궁극기 몽타주 재생
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && UltimateMontage)
	{
		AnimInstance->StopAllMontages(1);
		AnimInstance->Montage_Play(UltimateMontage);
	}

	// 독 효과 적용 (초당 공격력 5% 추가 피해, 4초 지속)
	if (UBuffComponent* BuffComp = FindComponentByClass<UBuffComponent>())
	{
		FBuffInfo PoisonBuff;
		PoisonBuff.BuffType = EBuffType::Poison;
		PoisonBuff.Duration = 4.0f;
		PoisonBuff.DamageOverTimePercent = 0.05f;
		BuffComp->AddBuff(PoisonBuff);
	}

	if (RatUltimateEffect)
	{
		UGameplayStatics::SpawnEmitterAttached(RatUltimateEffect, GetMesh(), FName("UltimateSocket"));
	}

	if (RatUltimateSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, RatUltimateSound, GetActorLocation());
	}

	UE_LOG(LogTemp, Warning, TEXT("1등 난타!!!!!"));
	
	// 쿨다운 시작 (예: 8초)
	bCanUseUltimate = false;
	GetWorldTimerManager().SetTimer(UltimateCooldownTimerHandle, [this]()
		{
			bCanUseUltimate = true;
			UE_LOG(LogTemp, Warning, TEXT("RatKnight 궁극기 사용 가능!"));
		}, 8.0f, false);
}


