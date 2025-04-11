#include "BuffComponent.h"
#include "PlayerCharacter.h"

UBuffComponent::UBuffComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UBuffComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UBuffComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	UpdateBuffs(DeltaTime);
}

void UBuffComponent::AddBuff(const FBuffInfo& NewBuff)
{
	ActiveBuffs.Add(NewBuff);

	UE_LOG(LogTemp, Warning, TEXT("Buff Added: Type=%d, Duration=%.2f"), (int32)NewBuff.BuffType, NewBuff.Duration);
}

void UBuffComponent::RemoveBuff(EBuffType BuffType)
{
	ActiveBuffs.RemoveAll([BuffType](const FBuffInfo& Buff)
		{
			return Buff.BuffType == BuffType;
		});
	UE_LOG(LogTemp, Warning, TEXT("Buff Removed: Type=%d"), (int32)BuffType);
}

void UBuffComponent::UpdateBuffs(float DeltaTime)
{
	// 역순 순회하여 만료된 버프 제거 & 도트뎀지 적용
	for (int32 i = ActiveBuffs.Num() - 1; i >= 0; i--)
	{
		ActiveBuffs[i].ElapsedTime += DeltaTime;

		// 도트 데미지 적용 : Poison 또는 Bleed 디버프일 경우, 적용!
		if ((ActiveBuffs[i].BuffType == EBuffType::Poison || ActiveBuffs[i].BuffType == EBuffType::Bleed) && ActiveBuffs[i].DamageOverTimePercent > 0.f)
		{
			//GetOwner()는 버프가 부착된(디버프를 받은) 캐릭터가 됨.
			APlayerCharacter* TargetCharacter = Cast<APlayerCharacter>(GetOwner());
			if (TargetCharacter && TargetCharacter->GetStatComponent())
			{
				// 공격자는 독/출혈 디버프를 적용할 때 버프에 저장한 SourceAttackValue를 사용
				float DotDamage = ActiveBuffs[i].SourceAttackValue * 
					ActiveBuffs[i].DamageOverTimePercent *
					DeltaTime;
				TargetCharacter->GetStatComponent()->ChangeHP(-DotDamage);
			}
		}
		// 버프 만료 처리
		if (ActiveBuffs[i].ElapsedTime >= ActiveBuffs[i].Duration)
		{
			UE_LOG(LogTemp, Warning, TEXT("Buff Expired: Type=%d"), (int32)ActiveBuffs[i].BuffType);
			ActiveBuffs.RemoveAt(i);
		}
	}
}

float UBuffComponent::GetAttackMultiplier() const
{
	float Multiplier = 1.0f;
	for (const FBuffInfo& Buff : ActiveBuffs)
	{
		Multiplier *= Buff.AttackMultiplier;
	}
	UE_LOG(LogTemp, Warning, TEXT("Attack Multiplier: %.2f"), Multiplier);
	return Multiplier;
}

float UBuffComponent::GetDefenseMultiplier() const
{
	float Multiplier = 1.0f;
	for (const FBuffInfo& Buff : ActiveBuffs)
	{
		Multiplier *= Buff.DefenseMultiplier;
	}
	UE_LOG(LogTemp, Warning, TEXT("Defense Multiplier: %.2f"), Multiplier);
	return Multiplier;
}

float UBuffComponent::GetMoveSpeedMultiplier() const
{
	float Multiplier = 1.0f;
	for (const FBuffInfo& Buff : ActiveBuffs)
	{
		Multiplier *= Buff.MoveSpeedMultiplier;
	}
	UE_LOG(LogTemp, Warning, TEXT("Move Speed Multiplier: %.2f"), Multiplier);
	return Multiplier;
}



