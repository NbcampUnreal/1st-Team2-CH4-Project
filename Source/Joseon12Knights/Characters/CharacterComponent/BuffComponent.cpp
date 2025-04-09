#include "BuffComponent.h"

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
}

void UBuffComponent::RemoveBuff(EBuffType BuffType)
{
	ActiveBuffs.RemoveAll([BuffType](const FBuffInfo& Buff)
		{
			return Buff.BuffType == BuffType;
		});
}

void UBuffComponent::UpdateBuffs(float DeltaTime)
{
	// 역순 순회하여 만료된 버프 제거
	for (int32 i = ActiveBuffs.Num() - 1; i >= 0; i--)
	{
		ActiveBuffs[i].ElapsedTime += DeltaTime;
		if (ActiveBuffs[i].ElapsedTime >= ActiveBuffs[i].Duration)
		{
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
	return Multiplier;
}

float UBuffComponent::GetDefenseMultiplier() const
{
	float Multiplier = 1.0f;
	for (const FBuffInfo& Buff : ActiveBuffs)
	{
		Multiplier *= Buff.DefenseMultiplier;
	}
	return Multiplier;
}

float UBuffComponent::GetMoveSpeedMultiplier() const
{
	float Multiplier = 1.0f;
	for (const FBuffInfo& Buff : ActiveBuffs)
	{
		Multiplier *= Buff.MoveSpeedMultiplier;
	}
	return Multiplier;
}



