
#include "OxKnight.h"
#include "EnhancedInputComponent.h"

// ============
// W Skill Logic
// ============
void AOxKnight::Skill(const FInputActionValue& Value)
{
	ServerSkill();
}

void AOxKnight::ServerSkill_Implementation()
{
	MulticastSkill();
}

void AOxKnight::MulticastSkill_Implementation()
{
	PrimaryActorTick.bCanEverTick = true;
	if (!bCanUseSkill)
	{
		UE_LOG(LogTemp, Warning, TEXT("OxKnight W Skill is CoolDown.."));
		return;
	}

	// �켱, SkillAttackMontage�� ���
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && SkillAttackMontage)
	{
		AnimInstance->StopAllMontages(1);
		AnimInstance->Montage_Play(SkillAttackMontage);

		// SkillAttackMontage�� ��� ���̸�ŭ ��� ��, ���� �ִϸ��̼�(PersistentSkillMontage)�� ���.
		float AttackMontageLength = SkillAttackMontage->GetPlayLength();
		GetWorldTimerManager().SetTimer(PlayPersistentMontageTimerHandle, this, &AOxKnight::PlayPersistentSkillMontage, AttackMontageLength, false);
	}

	// ����, ��� ���� ����
	if (UBuffComponent* BuffComp = FindComponentByClass<UBuffComponent>())
	{
		FBuffInfo GuardianBuff;
		GuardianBuff.BuffType = EBuffType::DefenseBuff;
		GuardianBuff.Duration = 4.0f;
		GuardianBuff.MoveSpeedMultiplier = 0.5f;
		GuardianBuff.DefenseMultiplier = 1.4f; // 40% ���� ����
		BuffComp->AddBuff(GuardianBuff);
		UE_LOG(LogTemp, Warning, TEXT("OxKnight W Skill is Succeed!"));
	}

	bCanUseSkill = false;
	GetWorldTimerManager().SetTimer(SkillTimerHandle, [this]()
		{
			bCanUseSkill = true;
			UE_LOG(LogTemp, Warning, TEXT("OxKnight W Skill is available"));
		}, SkillCoolTime, false); //5.0

	if (OxSkill1Effect)
	{
		UGameplayStatics::SpawnEmitterAttached(OxSkill1Effect, GetMesh(), FName("SkillSocket"));
	}

	if (OxSkill1Sound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, OxSkill1Sound, GetActorLocation());
	}

	UE_LOG(LogTemp, Warning, TEXT("Guard!!!"));

	// ��ų ���� �ð�(��: 4��) �Ŀ� ���� �ִϸ��̼��� �ߴ��ϴ� Ÿ�̸� ����
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
	ServerUltimate();
}

void AOxKnight::ServerUltimate_Implementation()
{
	MulticastUltimate();
}

void AOxKnight::MulticastUltimate_Implementation()
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

	// �̹� ó���� Ÿ�� ��� �ʱ�ȭ
	ProcessedTargets.Empty();

	FVector LaunchVelocity = GetActorForwardVector() * UltimateLaunchForce;
	LaunchCharacter(LaunchVelocity, true, false);

	// ���� �� �浹 �˻縦 ���� Ÿ�̸� ���� (�� ���� ����ǵ���)
	GetWorldTimerManager().ClearTimer(UltimateCollisionTimerHandle);
	GetWorldTimerManager().SetTimer(UltimateCollisionTimerHandle, this, &AOxKnight::PerformUltimateCollisionCheck, 0.1f, false);

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
	GetWorldTimerManager().SetTimer(UltimateTimerhandle, [this]()
		{
			bCanUseUltimate = true;
			UE_LOG(LogTemp, Warning, TEXT("OxKnight Ultimate is available!"));
		}, UltimateCoolTime, false); // 5.0f
}

// ���� �� �浹 �˻縦 �����ϴ� �Լ�
void AOxKnight::PerformUltimateCollisionCheck()
{
	// ���� ��ġ �������� ���� ��ü ����
	FVector Start = GetActorLocation();
	FVector End = Start + GetActorForwardVector() * UltimateLaunchForce * 0.5f;

	TArray<FHitResult> HitResults;
	FCollisionShape SphereShape = FCollisionShape::MakeSphere(70.0f);
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	// ����� �ð�ȭ
	//DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 5.0f, 0, 2.0f);
	//DrawDebugSphere(GetWorld(), End, 200.0f, 16, FColor::Blue, false, 5.0f);

	bool bHit = GetWorld()->SweepMultiByChannel(
		HitResults,
		Start,
		End,
		FQuat::Identity,
		ECC_Pawn,
		SphereShape,
		QueryParams
	);

	int32 AppliedCount = 0;

	for (const FHitResult& Hit : HitResults)
	{
		APlayerCharacter* Target = Cast<APlayerCharacter>(Hit.GetActor());

		// ��ȿ�� Ÿ���̸鼭 ������ ó������ ���� Ÿ�ٸ� ó��
		if (Target && Target != this && !ProcessedTargets.Contains(Target))
		{
			ProcessedTargets.Add(Target);

			if (UBuffComponent* TargetBuffComp = Target->FindComponentByClass<UBuffComponent>())
			{
				FBuffInfo StunBuff;
				StunBuff.BuffType = EBuffType::Slow;
				StunBuff.Duration = 4.0f;
				StunBuff.MoveSpeedMultiplier = 0.1f;
				TargetBuffComp->AddBuff(StunBuff);

				// ����� ���� ���
				DebugPrintTargetBuffs(Target);
				AppliedCount++;
			}
		}
	}
}


AOxKnight::AOxKnight()
{
	bReplicates = true;
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
	if (!Target)
	{
		UE_LOG(LogTemp, Warning, TEXT("DebugPrintTargetBuffs: Target is nullptr"));
		return;
	}

	if (UBuffComponent* TargetBuffComp = Target->FindComponentByClass<UBuffComponent>())
	{
		float MoveSpeedMult = TargetBuffComp->GetMoveSpeedMultiplier();

		if (GEngine)
		{
			FString DebugMsg = FString::Printf(TEXT("Target %s MoveSpeed: %.2f"), *Target->GetName(), MoveSpeedMult);
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, DebugMsg);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Target %s has no BuffComponent"), *Target->GetName());
	}
}