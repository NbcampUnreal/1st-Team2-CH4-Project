

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

	// �ִ� ��Ÿ�� ���
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && SkillAttackMontage)
	{
		AnimInstance->StopAllMontages(1);
		AnimInstance->Montage_Play(SkillAttackMontage);
	}

	// ȣ������ �г� ���� (���ݷ�, �̵��ӵ� ����)
	if (UBuffComponent* BuffComp = FindComponentByClass<UBuffComponent>())
	{
		// ���ݷ� ���� ����
		FBuffInfo AttackBuff;
		AttackBuff.BuffType = EBuffType::AttackBuff;
		AttackBuff.AttackMultiplier = 1.30f;  // +30% ���ݷ�
		AttackBuff.Duration = 5.0f;
		BuffComp->AddBuff(AttackBuff);

		// �̵��ӵ� ���� ����
		FBuffInfo SpeedBuff;
		// ���� ������ Ÿ���� ����ϰ� �ʹٸ� EBuffType::SpeedBuff�� �߰��ϰ� ���.
		SpeedBuff.BuffType = EBuffType::AttackBuff;
		SpeedBuff.MoveSpeedMultiplier = 2.0f; // +30% �̵��ӵ�
		SpeedBuff.Duration = 5.0f;
		BuffComp->AddBuff(SpeedBuff);

		UE_LOG(LogTemp, Warning, TEXT("TigerKnight W Skill Succeed!"));
	}

	// ��ٿ� ����
	bCanUseSkill = false;  
	GetWorldTimerManager().SetTimer(SkillCooldownTimerHandle, [this]()
		{
			bCanUseSkill = true;
			UE_LOG(LogTemp, Warning, TEXT("TigerKnight W Skill is available"));
		}, 7.0f, false);

	if (TigerSkill1Effect)
	{
		UGameplayStatics::SpawnEmitterAttached(TigerSkill1Effect,     //����� ��ƼŬ ����Ʈ
			GetMesh(),   //��� ������
			FName("SkillSocket"));   //� ���Ͽ� ������
	}

	if (TigerSkill1Sound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, TigerSkill1Sound, GetActorLocation());
	}
	UE_LOG(LogTemp, Warning, TEXT("AUHUNG!!"));

	DebugPrintSelfBuffs(); // ���� ���� ���� ���
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

	// �ñر� ��Ÿ�� ���
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && UltimateMontage)
	{
		AnimInstance->StopAllMontages(1);
		AnimInstance->Montage_Play(UltimateMontage);
	}

	// Ÿ���� ��뿡�� ���� ȿ�� ���� (������ ���� ����, ��Ʈ ������ & �� ���� ���� 4�� ����)
	APlayerCharacter* Target = GetTargetPlayer();
	if (Target) 
	{
		if (UBuffComponent* TargetBuffComp = Target -> FindComponentByClass<UBuffComponent>())
		{
			FBuffInfo BleedDebuff;
			BleedDebuff.BuffType = EBuffType::Bleed;
			BleedDebuff.Duration = 4.0f;
			BleedDebuff.DamageOverTimePercent = 0.1f; // ���� ���ϸ� ��� �ұ�? 1)AI ���� ������ ? 2)ĳ���� ���ݷ� �ø���?
			BleedDebuff.DefenseMultiplier = 0.8f;
			TargetBuffComp->AddBuff(BleedDebuff);
			
			// ���� �������� ��ų ������� ���ݷ¿� ����ϹǷ�, ������(=this)�� StatComponent���� ���� ������ ����
			if (this->GetStatComponent())
			{
				BleedDebuff.SourceAttackValue = this->GetStatComponent()->GetAttack();
			}

			TargetBuffComp->AddBuff(BleedDebuff);
			UE_LOG(LogTemp, Warning, TEXT("TigerKnight Ultimate is Succeed!"));


			// Ÿ�� ��ġ���� ����Ʈ ��� (�� �Ǵ� ����)
			if (TigerBleedNiagaraEffect && Target)
			{
				UNiagaraFunctionLibrary::SpawnSystemAttached(
					TigerBleedNiagaraEffect,
					Target->GetRootComponent(),			// ���� ��� ������Ʈ
					NAME_None,										// ���� �̸� (�ʿ��ϸ� ����)
					FVector::ZeroVector,							// ����� ��ġ ������ (�ʿ信 ���� ����)
					FRotator::ZeroRotator,							// ����� ȸ�� ������ (�ʿ信 ���� ����)
					EAttachLocation::KeepRelativeOffset,	// ���� ��ġ ��Ģ
					true														// auto destroy
				);
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("TigerKnight �ñر� Ÿ�� ����!"));
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

	// ��ٿ� ����
	bCanUseUltimate = false;
	GetWorldTimerManager().SetTimer(UltimateCooldownTimerHandle, [this]()
		{
			bCanUseUltimate = true;
			UE_LOG(LogTemp, Warning, TEXT("TigerKnight Ultimate is available!"));
		}, 8.0f, false);
}

// ==========
//  ����� �Լ�
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


	
