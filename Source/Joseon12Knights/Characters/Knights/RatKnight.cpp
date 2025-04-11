
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

// ======================
// RatKnight Targeting Logic
// ======================
APlayerCharacter* ARatKnight::GetTargetPlayer()
{
	FVector AttackOrigin; // 공격 시작 지점

	// 무기가 있으면 무기 위치를 기준으로 설정
	if (WeaponComponent && WeaponComponent->DoesSocketExist(FName("WeaponSocket")))
	{
		AttackOrigin = WeaponComponent->GetSocketLocation(FName("WeaponSocket"));
	}
	// 없으면 캐릭터 전방에 오프셋 적용
	else
	{
		AttackOrigin = GetActorLocation() + GetActorForwardVector() * ForwardOffset;
	}

	// 공격 범위 내의 모든 캐릭터를 찾기
	TArray<APlayerCharacter*> PotentialTargets = FindTargetsInRadius(AttackOrigin, AttackRadius);

	// 가장 적합한 타겟 선택
	APlayerCharacter* BestTarget = SelectBestTarget(PotentialTargets);

	// 디버그 시각화
	if (bDebugTargeting)
	{
		// 공격 반경 시각화
		DrawDebugSphere(GetWorld(), AttackOrigin, AttackRadius, 24,
			BestTarget ? FColor::Green : FColor::Red, false, 1.0f);

		// 선택된 타겟 표시
		if (BestTarget)
		{
			DrawDebugLine(GetWorld(), AttackOrigin, BestTarget->GetActorLocation(),
				FColor::Yellow, false, 1.0f, 0, 2.0f);
		}
	}

	return BestTarget; // 임시로 널 반환
}

TArray<APlayerCharacter*> ARatKnight::FindTargetsInRadius(const FVector& Origin, float Radius)
{
	TArray<APlayerCharacter*> FoundTargets;
	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this); // 자신은 제외

	// 구체 오버랩 검사 수행
	bool bOverlapFound = GetWorld()->OverlapMultiByObjectType(
		OverlapResults,
		Origin,
		FQuat::Identity,
		FCollisionObjectQueryParams(TargetCollisionChannel), // 설정한 채널에 따라 감지
		FCollisionShape::MakeSphere(Radius),
		QueryParams
	);

	if (bOverlapFound)
	{
		// 결과에서 APlayerCharacter 타입만 필터링
		for (const FOverlapResult& Overlap : OverlapResults)
		{
			APlayerCharacter* PlayerChar = Cast<APlayerCharacter>(Overlap.GetActor());
			if (PlayerChar && PlayerChar != this)
			{
				// 팀 기반 게임이라면 적 팀만 타겟팅하도록 필터링 가능
				// if (PlayerChar->GetTeam() != GetTeam()) 
				// {
				FoundTargets.Add(PlayerChar);
				// }
			}
		}
	}

	return FoundTargets;
}

APlayerCharacter* ARatKnight::SelectBestTarget(const TArray<APlayerCharacter*>& PotentialTargets)
{
	if (PotentialTargets.Num() == 0)
	{
		return nullptr;
	}

	// 타겟 선택 로직:
	// 1. 우선순위: 전방 시야각 내에 있는 적
	// 2. 차선책: 가장 가까운 적

	APlayerCharacter* BestTarget = nullptr;
	float BestScore = -1.0f;

	FVector Forward = GetActorForwardVector();
	FVector ActorLocation = GetActorLocation();

	const float ViewAngleCos = FMath::Cos(FMath::DegreesToRadians(60.0f)); // 60도 시야각
	const float ViewAngleWeight = 2.0f; // 시야각 내 대상 가중치

	for (APlayerCharacter* Target : PotentialTargets)
	{
		if (!Target)
		{
			continue;
		}

		FVector ToTarget = Target->GetActorLocation() - ActorLocation;
		float Distance = ToTarget.Size();

		// 거리가 멀수록 점수 감소 (역수 사용)
		float DistanceScore = 1.0f / FMath::Max(Distance, 1.0f);

		// 방향 정규화
		ToTarget.Normalize();

		// 전방 벡터와 타겟 방향 사이의 각도 계산 (내적 사용)
		float DotProduct = FVector::DotProduct(Forward, ToTarget);

		// 시야각 내에 있는지 확인 (DotProduct > ViewAngleCos면 시야각 내에 있음)
		float AngleScore = DotProduct > ViewAngleCos ? ViewAngleWeight : 1.0f;

		// 최종 점수 계산
		float FinalScore = DistanceScore * AngleScore;

		// 더 높은 점수의 타겟이면 갱신
		if (FinalScore > BestScore)
		{
			BestScore = FinalScore;
			BestTarget = Target;
		}
	}

	return BestTarget;
}


// ====================
// RatKnight W Skill Logic
// ====================
void ARatKnight::Skill(const FInputActionValue& Value)
{
	// 쥐기사 W 스킬: 『쥐구멍』
	// 이중 효과 :
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
		if (UBuffComponent* TargetBuffComp = FindComponentByClass<UBuffComponent>())
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
	if (UBuffComponent* SelfBuffComp = FindComponentByClass<UBuffComponent>())
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

		UE_LOG(LogTemp, Warning, TEXT("쥐구멍 은신 효과 적용! (지속시간: %f초)"),StealthDuration);
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
	GetWorldTimerManager().SetTimer(SkillCooldownTimerHandle, [this]()
		{
			bCanUseSkill = true;
			UE_LOG(LogTemp, Warning, TEXT("RatKnight W 스킬 사용 가능!"));
		}, 8.0f, false);
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
		if (UBuffComponent* TargetBuffComp = FindComponentByClass<UBuffComponent>())
		{
			FBuffInfo PoisonDebuff;
			PoisonDebuff.BuffType = EBuffType::Poison;
			PoisonDebuff.Duration = 4.0f;
			PoisonDebuff.DamageOverTimePercent = 0.05f; // 초당 5% 공격력에 해당하는 도트 효과
			TargetBuffComp->AddBuff(PoisonDebuff);
		}
		UE_LOG(LogTemp, Warning, TEXT("RatKnight 궁극기 타격 성공!"));
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
	GetWorldTimerManager().SetTimer(UltimateCooldownTimerHandle, [this]()
		{
			bCanUseUltimate = true;
			UE_LOG(LogTemp, Warning, TEXT("RatKnight 궁극기 사용 가능!"));
		}, 8.0f, false);
}
