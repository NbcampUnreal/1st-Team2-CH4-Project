


#include "OxKnight.h"
#include "EnhancedInputComponent.h"

void AOxKnight::Skill(const FInputActionValue& Value)
{
	if(!bCanUseSkill)
	{
		UE_LOG(LogTemp, Warning, TEXT("OxKnight W is CoolDown.."));
		return;
	}

	// 애님 몽타주 재생
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && SkillAttackMontage)
	{
		AnimInstance->StopAllMontages(1);
		AnimInstance->Montage_Play(SkillAttackMontage);
	}

	// 철벽 수호(방어력 증가 + 이속감소)
	if (UBuffComponent* BuffComp = FindComponentByClass<UBuffComponent>())
	{

		FBuffInfo GuardianBuff;
		GuardianBuff.BuffType = EBuffType::DefenseBuff;
		GuardianBuff.Duration = 4.0f;
		GuardianBuff.MoveSpeedMultiplier = 0.7f;  //이속감소
		GuardianBuff.DefenseMultiplier = 0.6f; //40% 피해감소
		BuffComp->AddBuff(GuardianBuff);

		
	}

	// 쿨다운 시작 (예: 5초)
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
	// 전방으로 돌진해 스턴과 넉백효과 
	if (!bCanUseUltimate)
	{
		UE_LOG(LogTemp, Warning, TEXT("OxKnight ultimate is CoolDown..."));
		return;
	}

	// 궁극기 몽타주 재생
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && UltimateMontage)
	{
		AnimInstance->StopAllMontages(1);
		AnimInstance->Montage_Play(UltimateMontage);
	}

	//스턴과 넉백
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

	// 쿨다운 시작 (예: 8초)
	bCanUseUltimate = false;
	GetWorldTimerManager().SetTimer(UltimateCooldownTimerHandle, [this]()
		{
			bCanUseUltimate = true;
			UE_LOG(LogTemp, Warning, TEXT("OxKnight Ultimate is available!"));
		}, 8.0f, false);
}

