// 


#include "StatComponent.h"


// Sets default values for this component's properties
UStatComponent::UStatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UStatComponent::InitializeStats(const FCharacterStatInfo& NewStats)
{
	BaseStats = NewStats;
	CurrentHP = BaseStats.MaxHP;
}

void UStatComponent::ChangeHP(float Delta)
{
	CurrentHP = FMath::Clamp(CurrentHP + Delta, 0.0f, BaseStats.MaxHP);
	if (CurrentHP <= 0.0f)
	{
		// HP가 0 이하가 되면 사망 이벤트 발생
		OnDeath.Broadcast();
	}
}

void UStatComponent::BeginPlay()
{
	Super::BeginPlay();
	CurrentHP = BaseStats.MaxHP;
}
