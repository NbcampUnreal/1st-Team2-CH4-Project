
#include "OxKnight.h"
#include "EnhancedInputComponent.h"

// ============
// W Skill Logic
// ============
void AOxKnight::Skill(const FInputActionValue& Value)
{
	if (!bCanUseSkill)
	{
		UE_LOG(LogTemp, Warning, TEXT("OxKnight W Skill is CoolDown.."));
		return;
	}

	// 우선, SkillAttackMontage를 재생
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && SkillAttackMontage)
	{
		AnimInstance->StopAllMontages(1);
		AnimInstance->Montage_Play(SkillAttackMontage);

		// SkillAttackMontage의 재생 길이만큼 대기 후, 지속 애니메이션(PersistentSkillMontage)를 재생.
		float AttackMontageLength = SkillAttackMontage->GetPlayLength();
		GetWorldTimerManager().SetTimer(PlayPersistentMontageTimerHandle, this, &AOxKnight::PlayPersistentSkillMontage, AttackMontageLength, false);
	}

	// 이후, 방어 버프 적용
	if (UBuffComponent* BuffComp = FindComponentByClass<UBuffComponent>())
	{
		FBuffInfo GuardianBuff;
		GuardianBuff.BuffType = EBuffType::DefenseBuff;
		GuardianBuff.Duration = 4.0f;
		GuardianBuff.MoveSpeedMultiplier = 0.5f;
		GuardianBuff.DefenseMultiplier = 1.4f; // 40% 방어력 증가
		BuffComp->AddBuff(GuardianBuff);
		UE_LOG(LogTemp, Warning, TEXT("OxKnight W Skill is Succeed!"));
	}

	bCanUseSkill = false;
	GetWorldTimerManager().SetTimer(SkillCooldownTimerHandle, [this]()
		{
			bCanUseSkill = true;
			UE_LOG(LogTemp, Warning, TEXT("OxKnight W Skill is available"));
		}, 5.0f, false);

	if (OxSkill1Effect)
	{
		UGameplayStatics::SpawnEmitterAttached(OxSkill1Effect, GetMesh(), FName("SkillSocket"));
	}

	if (OxSkill1Sound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, OxSkill1Sound, GetActorLocation());
	}

	UE_LOG(LogTemp, Warning, TEXT("Guard!!!"));

	// 스킬 지속 시간(예: 4초) 후에 지속 애니메이션을 중단하는 타이머 설정
	GetWorldTimerManager().ClearTimer(StopAnimationTimerHandle);
	GetWorldTimerManager().SetTimer(StopAnimationTimerHandle, this, &AOxKnight::StopPersistentSkillAnimation, 4.0f, false);

	DebugPrintSelfBuffs();
}

void AOxKnight::PlayPersistentSkillMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && PersistentSkillMontage)
	{
		AnimInstance->Montage_Play(PersistentSkillMontage);
		UE_LOG(LogTemp, Warning, TEXT("Persistent Skill Animation Started."));
	}
}

void AOxKnight::StopPersistentSkillAnimation()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && PersistentSkillMontage)
	{
		AnimInstance->Montage_Stop(0.2f, PersistentSkillMontage);
		UE_LOG(LogTemp, Warning, TEXT("Persistent skill animation stopped."));
	}
}

// ==============
// R Ultimate Logic
// ==============
void AOxKnight::Ultimate(const FInputActionValue& Value)
{
	// 전방으로 돌진해 스턴과 넉백효과 
	if (!bCanUseUltimate)
	{
		UE_LOG(LogTemp, Warning, TEXT("OxKnight ultimate is CoolDown..."));
		return;
	}

	// 돌진 효과 :: 캐릭터가 바라보는 정확한 전방 벡터 계산
	// GetActorRotation()을 사용해 로컬 X축을 계산하고, 필요할 때 반전!!
	FRotator ActorRotation = GetActorRotation();
	FVector CalculatedForward = FRotationMatrix(ActorRotation).GetUnitAxis(EAxis::X);
	FVector FinalForward = CalculatedForward;
	FVector LaunchVelocity = FinalForward * UltimateLaunchForce;
	LaunchCharacter(LaunchVelocity, true, false); // XY 강제 적용, Z는 유지

	// 궁극기 몽타주 재생
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && UltimateMontage)
	{
		AnimInstance->StopAllMontages(1);
		AnimInstance->Montage_Play(UltimateMontage);
	}

	//타겟에게 스턴 적용
	APlayerCharacter* Target = GetTargetPlayer();
	if (Target)
	{
		if (UBuffComponent* BuffComp = Target->FindComponentByClass<UBuffComponent>())
		{
			FBuffInfo StunBuff;
			StunBuff.BuffType = EBuffType::Slow;
			StunBuff.Duration = 4.0f;
			StunBuff.MoveSpeedMultiplier = 0.1f; // 스턴효과 : 이동속도 0.1 즉, 거의 멈춘 상태
			BuffComp->AddBuff(StunBuff);
			
			UE_LOG(LogTemp, Warning, TEXT("OxKnight Ultimate is Succeed!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("OxKnight Ultimate is Failed!"));
	}
	if (OxUltimateEffect)
	{
		UGameplayStatics::SpawnEmitterAttached(OxUltimateEffect, GetMesh(), FName("UltimateSocket"));
	}

	if (OxUltimateSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, OxUltimateSound, GetActorLocation());
	}

	UE_LOG(LogTemp, Warning, TEXT("Fierce Rush!!!!!"));

	bCanUseUltimate = false;
	GetWorldTimerManager().SetTimer(UltimateCooldownTimerHandle, [this]()
		{
			bCanUseUltimate = true;
			UE_LOG(LogTemp, Warning, TEXT("OxKnight Ultimate is available!"));
		}, 8.0f, false);

	DebugPrintTargetBuffs(Target); // 타겟 버프 적용 상태 출력
}

void AOxKnight::DebugPrintSelfBuffs() const
{
	if (UBuffComponent* BuffComp = const_cast<AOxKnight*>(this)->FindComponentByClass<UBuffComponent>())
	{
		float CurrentDefenceMult = BuffComp->GetDefenseMultiplier();
		float CurrentMoveSpeedMult = BuffComp->GetMoveSpeedMultiplier();

		if (GEngine)
		{
			FString DebugMsg = FString::Printf(TEXT("Self: Defense=%.2f, MoveSpeed=%.2f"),
				CurrentDefenceMult, CurrentMoveSpeedMult);
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, DebugMsg);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("OxKnight BuffComponent not found on %s!"), *GetName());
	}
}

void AOxKnight::DebugPrintTargetBuffs(APlayerCharacter* Target) const
{
	if (Target)
	{
		if (UBuffComponent* TargetBuffComp = Target->FindComponentByClass<UBuffComponent>())
		{
			float MoveSpeedMult = TargetBuffComp->GetMoveSpeedMultiplier();
			if (GEngine)
			{
				FString DebugMsg = FString::Printf(TEXT("Target %s MoveSpeed: %.2f"), *Target->GetName(), MoveSpeedMult);
				GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, DebugMsg);
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Target %s has no BuffComponent"), *Target->GetName());
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Target is nullptr"));
	}
}