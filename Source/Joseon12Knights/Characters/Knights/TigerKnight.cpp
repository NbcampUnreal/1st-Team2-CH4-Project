

#include "TigerKnight.h"
#include "EnhancedInputComponent.h"


// ====================
// TigerKnight W Skill Logic
// ====================
void ATigerKnight::Skill(const FInputActionValue& Value)
{
	if (!bCanUseSkill)
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

	// 호랑이의 분노 적용 (공격력, 이동속도 증가)
	if (UBuffComponent* BuffComp = FindComponentByClass<UBuffComponent>())
	{
		// 공격력 증가 버프
		FBuffInfo AttackBuff;
		AttackBuff.BuffType = EBuffType::AttackBuff;
		AttackBuff.AttackMultiplier = 1.30f;  // +30% 공격력
		AttackBuff.Duration = 5.0f;
		BuffComp->AddBuff(AttackBuff);

		// 이동속도 증가 버프
		FBuffInfo SpeedBuff;
		// 만약 별도의 타입을 사용하고 싶다면 EBuffType::SpeedBuff를 추가하고 사용.
		SpeedBuff.BuffType = EBuffType::AttackBuff;
		SpeedBuff.MoveSpeedMultiplier = 2.0f; // +30% 이동속도
		SpeedBuff.Duration = 5.0f;
		BuffComp->AddBuff(SpeedBuff);

		UE_LOG(LogTemp, Warning, TEXT("TigerKnight W Skill Succeed!"));
	}

	// 쿨다운 시작
	bCanUseSkill = false;  
	GetWorldTimerManager().SetTimer(SkillCooldownTimerHandle, [this]()
		{
			bCanUseSkill = true;
			UE_LOG(LogTemp, Warning, TEXT("TigerKnight W Skill is available"));
		}, 7.0f, false);

	if (TigerSkill1Effect)
	{
		UGameplayStatics::SpawnEmitterAttached(TigerSkill1Effect,     //재생할 파티클 이펙트
			GetMesh(),   //어디에 붙일지
			FName("SkillSocket"));   //어떤 소켓에 붙일지
	}

	if (TigerSkill1Sound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, TigerSkill1Sound, GetActorLocation());
	}
	UE_LOG(LogTemp, Warning, TEXT("AUHUNG!!"));

	DebugPrintSelfBuffs(); // 버프 적용 상태 출력
}

// ====================
// TigerKnight R Skill Logic
// ====================
void ATigerKnight::Ultimate(const FInputActionValue& Value)
{
	if (!bCanUseUltimate)
	{
		UE_LOG(LogTemp, Warning, TEXT("TigerKnight Ultimate is CoolDown..."));
		return;
	}

	// 궁극기 몽타주 재생
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && UltimateMontage)
	{
		AnimInstance->StopAllMontages(1);
		AnimInstance->Montage_Play(UltimateMontage);
	}

	// 타격한 상대에게 출혈 효과 적용 (강력한 베기 공격, 도트 데미지 & 적 방어력 저하 4초 지속)
	APlayerCharacter* Target = GetTargetPlayer();
	if (Target) 
	{
		if (UBuffComponent* TargetBuffComp = Target -> FindComponentByClass<UBuffComponent>())
		{
			FBuffInfo BleedDebuff;
			BleedDebuff.BuffType = EBuffType::Bleed;
			BleedDebuff.Duration = 4.0f;
			BleedDebuff.DamageOverTimePercent = 0.1f; // 방어력 저하를 어떻게 할까? 1)AI 방어력 내리기 ? 2)캐락터 공격력 늘리기?
			BleedDebuff.DefenseMultiplier = 0.8f;
			TargetBuffComp->AddBuff(BleedDebuff);
			
			// 출혈 데미지는 스킬 사용자의 공격력에 기반하므로, 공격자(=this)의 StatComponent에서 값을 가져와 저장
			if (this->GetStatComponent())
			{
				BleedDebuff.SourceAttackValue = this->GetStatComponent()->GetAttack();
			}

			TargetBuffComp->AddBuff(BleedDebuff);
			UE_LOG(LogTemp, Warning, TEXT("TigerKnight Ultimate is Succeed!"));


			// 타겟 위치에서 이펙트 재생 (독 또는 출혈)
			if (TigerBleedNiagaraEffect && Target)
			{
				UNiagaraFunctionLibrary::SpawnSystemAttached(
					TigerBleedNiagaraEffect,
					Target->GetRootComponent(),			// 부착 대상 컴포넌트
					NAME_None,										// 소켓 이름 (필요하면 지정)
					FVector::ZeroVector,							// 상대적 위치 오프셋 (필요에 따라 조정)
					FRotator::ZeroRotator,							// 상대적 회전 오프셋 (필요에 따라 조정)
					EAttachLocation::KeepRelativeOffset,	// 부착 위치 규칙
					true														// auto destroy
				);
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("TigerKnight 궁극기 타격 실패!"));
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

	// 쿨다운 시작
	bCanUseUltimate = false;
	GetWorldTimerManager().SetTimer(UltimateCooldownTimerHandle, [this]()
		{
			bCanUseUltimate = true;
			UE_LOG(LogTemp, Warning, TEXT("TigerKnight Ultimate is available!"));
		}, 8.0f, false);
}

// ==========
//  디버그 함수
// ==========
void ATigerKnight::DebugPrintSelfBuffs() const
{
	if (UBuffComponent* BuffComp = const_cast<ATigerKnight*>(this)->FindComponentByClass<UBuffComponent>())
	{
		float CurrentAttackMult = BuffComp->GetAttackMultiplier();
		float CurrentMoveSpeedMult = BuffComp->GetMoveSpeedMultiplier();

		if (GEngine)
		{
			FString DebugMsg = FString::Printf(TEXT("Attack: %.2f, MoveSpeed: %.2f"), CurrentAttackMult, CurrentMoveSpeedMult);
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, DebugMsg);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("BuffComponent not found on %s"), *GetName());
	}
}


	
