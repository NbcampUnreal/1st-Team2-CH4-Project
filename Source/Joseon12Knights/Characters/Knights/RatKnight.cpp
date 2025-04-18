
#include "RatKnight.h"
#include "EnhancedInputComponent.h"

void ARatKnight::BeginPlay()
{
	Super::BeginPlay();

	// 시작 시 원본 머티리얼 저장
	if (OriginalMaterials.Num() == 0)
	{
		TArray<UMaterialInterface*> MeshMaterials = GetMesh()->GetMaterials();
		OriginalMaterials = MeshMaterials;
	}

	if (WeaponComponent && OriginalWeaponMaterials.Num() == 0)
	{
		TArray<UMaterialInterface*> WeaponMats = WeaponComponent->GetMaterials();
		OriginalWeaponMaterials = WeaponMats;
	}

	if (ShieldComponent && OriginalShieldMaterials.Num() == 0)
	{
		TArray<UMaterialInterface*> ShieldMats = ShieldComponent->GetMaterials();
		OriginalShieldMaterials = ShieldMats;
	}
}

// ====================
// RatKnight W Skill Logic
// ====================
void ARatKnight::Skill(const FInputActionValue& Value)
{
	ServerSkill();
}

void ARatKnight::ServerSkill_Implementation()
{
	MulticastSkill();
}

void ARatKnight::MulticastSkill_Implementation()
{
	// 쥐기사 W 스킬: 『쥐구멍』
// 1) 공격형 디버프 : 타겟에게 슬로우 효과 적용 (타겟 이동속도 70%로 2초)
// 2) 은신 효과 : 쥐기사 본인에게 은신 효과 적용

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

	// 1) 타격한 상대에게 슬로우 디버프 적용 (이동속도 70%로 2초간 유지)
	APlayerCharacter* Target = GetTargetPlayer();
	if (Target)
	{
		if (UBuffComponent* TargetBuffComp = Target->FindComponentByClass<UBuffComponent>()) // 타겟에게 디버프 적용...!
		{
			FBuffInfo SlowDebuff;
			SlowDebuff.BuffType = EBuffType::Slow;
			SlowDebuff.Duration = 2.0f;
			SlowDebuff.MoveSpeedMultiplier = 0.7f; // -30% 속도
			TargetBuffComp->AddBuff(SlowDebuff);
		}
		UE_LOG(LogTemp, Warning, TEXT("RatKnight W 스킬 타격 성공!"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("RatKnight W 스킬 타격 실패!"));
	}

	// 2) 쥐기사 본인에게 은신 효과 적용 (5초간 은신)
	if (UBuffComponent* SelfBuffComp = FindComponentByClass<UBuffComponent>()) // 쥐기사 본인에게 은신 효과 적용
	{
		FBuffInfo StealthBuff;
		StealthBuff.BuffType = EBuffType::Stealth;
		StealthBuff.Duration = 5.0f;
		SelfBuffComp->AddBuff(StealthBuff);

		// 캐릭터 시각적 효과 적용 (반투명화)
		SetStealthVisibility(true);

		// 은신 효과 종료 타이머 설정
		GetWorldTimerManager().ClearTimer(StealthTimerHandle);
		GetWorldTimerManager().SetTimer(StealthTimerHandle, this, &ARatKnight::EndStealth, StealthBuff.Duration, false);

		UE_LOG(LogTemp, Warning, TEXT("쥐구멍 은신 효과 적용! (지속시간: %f초)"), StealthDuration);
	}

	// Skill Effect & Sound
	if (RatSkill1Effect)
	{
		UGameplayStatics::SpawnEmitterAttached(RatSkill1Effect, GetMesh(), FName("SkillSocket"));
	}

	if (RatSkill1Sound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, RatSkill1Sound, GetActorLocation());
	}

	UE_LOG(LogTemp, Warning, TEXT("쥐구멍!!!"));

	// Cooldown Start
	bCanUseSkill = false;
	GetWorldTimerManager().SetTimer(SkillTimerHandle, [this]()
		{
			bCanUseSkill = true;
			UE_LOG(LogTemp, Warning, TEXT("RatKnight W 스킬 사용 가능!"));
		}, SkillCoolTime, false);

	DebugPrintTargetBuffs(Target);
}

// 은신 상태에서 캐릭터 시각적 효과 설정
void ARatKnight::SetStealthVisibility(bool bIsStealthed)
{
	if (bIsStealthed)
	{
		// 은신 상태일 때 캐릭터 외형 변경
		GetMesh()->SetRenderCustomDepth(true);
		GetMesh()->SetCustomDepthStencilValue(1);

		// 미리 설정된 은신 머티리얼이 있으면 적용
		if (StealthMaterials.Num() > 0)
		{
			for (int32 i = 0; i < StealthMaterials.Num(); i++)
			{
				if (StealthMaterials[i] && i < GetMesh()->GetNumMaterials())
				{
					GetMesh()->SetMaterial(i, StealthMaterials[i]);
				}
			}
		}
		else
		{
			// 설정된 은신 머티리얼이 없으면 원본 머티리얼을 기반으로 반투명 처리
			UE_LOG(LogTemp, Warning, TEXT("StealthMaterials이 설정되지 않음. 블루프린트에서 반투명 머티리얼을 설정."));
		}

		// 무기 메시 머티리얼 변경
		if (WeaponComponent)
		{
			WeaponComponent->SetRenderCustomDepth(true);
			WeaponComponent->SetCustomDepthStencilValue(1);

			if (StealthWeaponMaterials.Num() > 0)
			{
				for (int32 i = 0; i < StealthWeaponMaterials.Num(); i++)
				{
					if (StealthWeaponMaterials[i] && i < WeaponComponent->GetNumMaterials())
					{
						WeaponComponent->SetMaterial(i, StealthWeaponMaterials[i]);
					}
				}
			}
			else if (StealthMaterials.Num() > 0)
			{
				// 전용 머티리얼이 없으면 캐릭터 스텔스 머티리얼을 재사용
				for (int32 i = 0; i < WeaponComponent->GetNumMaterials(); i++)
				{
					if (i < StealthMaterials.Num() && StealthMaterials[i])
					{
						WeaponComponent->SetMaterial(i, StealthMaterials[i]);
					}
				}
			}
		}

		// 방패 메시 머티리얼 변경
		if (ShieldComponent)
		{
			ShieldComponent->SetRenderCustomDepth(true);
			ShieldComponent->SetCustomDepthStencilValue(1);

			if (StealthShieldMaterials.Num() > 0)
			{
				for (int32 i = 0; i < StealthShieldMaterials.Num(); i++)
				{
					if (StealthShieldMaterials[i] && i < ShieldComponent->GetNumMaterials())
					{
						ShieldComponent->SetMaterial(i, StealthShieldMaterials[i]);
					}
				}
			}
			else if (StealthMaterials.Num() > 0)
			{
				// 전용 머티리얼이 없으면 캐릭터 스텔스 머티리얼을 재사용
				for (int32 i = 0; i < ShieldComponent->GetNumMaterials(); i++)
				{
					if (i < StealthMaterials.Num() && StealthMaterials[i])
					{
						ShieldComponent->SetMaterial(i, StealthMaterials[i]);
					}
				}
			}
		}

		// AI가 인식하지 못하도록 처리 (필요시 AI 컨트롤러에 추가 로직 필요)
		bIsInStealthMode = true;
	}
	else
	{
		// 원래 상태로 되돌림
		GetMesh()->SetRenderCustomDepth(false);

		// 캐릭터 메시 원본 머티리얼로 복원
		for (int32 i = 0; i < OriginalMaterials.Num(); i++)
		{
			if (OriginalMaterials[i] && i < GetMesh()->GetNumMaterials())
			{
				GetMesh()->SetMaterial(i, OriginalMaterials[i]);
			}
		}

		// 무기 메시 원본 머티리얼로 복원
		if (WeaponComponent)
		{
			WeaponComponent->SetRenderCustomDepth(false);

			for (int32 i = 0; i < OriginalWeaponMaterials.Num(); i++)
			{
				if (OriginalWeaponMaterials[i] && i < WeaponComponent->GetNumMaterials())
				{
					WeaponComponent->SetMaterial(i, OriginalWeaponMaterials[i]);
				}
			}
		}

		// 방패 메시 원본 머티리얼로 복원
		if (ShieldComponent)
		{
			ShieldComponent->SetRenderCustomDepth(false);

			for (int32 i = 0; i < OriginalShieldMaterials.Num(); i++)
			{
				if (OriginalShieldMaterials[i] && i < ShieldComponent->GetNumMaterials())
				{
					ShieldComponent->SetMaterial(i, OriginalShieldMaterials[i]);
				}
			}
		}

		// AI 인식 상태 복원
		bIsInStealthMode = false;
	}
}

// 은신 효과 종료 함수
void ARatKnight::EndStealth()
{
	SetStealthVisibility(false);

	if (StealthEndEffect)
	{
		UGameplayStatics::SpawnEmitterAttached(StealthEndEffect, GetMesh(), FName("RootSocket"));
	}
	if (StealthEndSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, StealthEndSound, GetActorLocation());
	}

	UE_LOG(LogTemp, Warning, TEXT("쥐구멍 은신 효과 종료!"));
}

// ====================
// RatKnight R Skill Logic
// ====================
void ARatKnight::Ultimate(const FInputActionValue& Value)
{
	ServerUltimate();
}

void ARatKnight::ServerUltimate_Implementation()
{
	MulticastUltimate();
}

void ARatKnight::MulticastUltimate_Implementation()
{
	// 쥐기사 R 궁극기: 『1등 난타』
// 공격형 디버프 : 타겟에게 독 효과 적용 (초당 5% 도트, 4초)
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

	// R키 입력 시 캐릭터를 살짝 공중으로 띄우기
	const float UltimateLaunchForce = 500.0f; // 상승력(?)
	const FVector LauchVelocity(0.0f, 0.0f, UltimateLaunchForce);
	LaunchCharacter(LauchVelocity, false, true); // bXYOverride = false (XY 속도 유지), bZOverride = true (Z축 속도 강제 적용)


	// 타격한 상대에게 독 디버프 적용 (초당 공격력 5% 추가 피해, 4초 지속)
	APlayerCharacter* Target = GetTargetPlayer();
	if (Target)
	{
		if (UBuffComponent* TargetBuffComp = Target->FindComponentByClass<UBuffComponent>()) // 타겟에게 디버프 적용
		{
			FBuffInfo PoisonDebuff;
			PoisonDebuff.BuffType = EBuffType::Poison;
			PoisonDebuff.Duration = 4.0f;
			PoisonDebuff.DamageOverTimePercent = 0.01f; // 초당 1% 공격력에 해당하는 도트 효과
			TargetBuffComp->AddBuff(PoisonDebuff);

			// 독 데미지는 스킬 사용자의 공격력에 기반하므로, 공격자(=this)의 StatComponent에서 값을 가져와 저장
			if (this->GetStatComponent())
			{
				PoisonDebuff.SourceAttackValue = this->GetStatComponent()->GetAttack();
			}

			TargetBuffComp->AddBuff(PoisonDebuff);
		}


		UE_LOG(LogTemp, Warning, TEXT("RatKnight 궁극기 타격 성공!"));

		// 타겟 위치에서 이펙트 재생 (독 또는 출혈)
		if (RatPoisonNiagaraEffect && Target)
		{
			UNiagaraFunctionLibrary::SpawnSystemAttached(
				RatPoisonNiagaraEffect,
				Target->GetRootComponent(),				// 부착 대상 컴포넌트
				NAME_None,											// 소켓 이름 (필요하면 지정)
				FVector::ZeroVector,								// 상대적 위치 오프셋 (필요에 따라 조정)
				FRotator::ZeroRotator,								// 상대적 회전 오프셋 (필요에 따라 조정)
				EAttachLocation::KeepRelativeOffset,		// 부착 위치 규칙
				true															// auto destroy
			);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("RatKnight 궁극기 타격 실패!"));
	}

	// Ultimate Effect & Sound
	if (RatUltimateEffect)
	{
		UGameplayStatics::SpawnEmitterAttached(RatUltimateEffect, GetMesh(), FName("UltimateSocket"));
	}

	if (RatUltimateSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, RatUltimateSound, GetActorLocation());
	}

	UE_LOG(LogTemp, Warning, TEXT("1등 난타!!!!!"));

	// Cooldown Start
	bCanUseUltimate = false;
	GetWorldTimerManager().SetTimer(UltimateTimerhandle, [this]()
		{
			bCanUseUltimate = true;
			UE_LOG(LogTemp, Warning, TEXT("RatKnight 궁극기 사용 가능!"));
		}, UltimateCoolTime, false);
}

// ===========
//  디버그 함수
// ===========
void ARatKnight::DebugPrintTargetBuffs(APlayerCharacter* Target) const
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