#include "StatComponent.h"

// Sets default values for this component's properties
UStatComponent::UStatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UStatComponent::BeginPlay()
{
	Super::BeginPlay();
	CurrentHP = BaseStats.MaxHP;
}

void UStatComponent::InitializeStats(const FCharacterStatInfo& NewStats)
{
	BaseStats = NewStats;
	CurrentHP = BaseStats.MaxHP;
}

void UStatComponent::ChangeHP(float Delta)
{
    float OldHP = CurrentHP; // 변경 전 HP 저장

    // HP 변경: 음수는 데미지, 양수는 회복
    CurrentHP = FMath::Clamp(CurrentHP + Delta, 0.0f, BaseStats.MaxHP);

    if (GEngine)
    {
        FString DebugMsg = FString::Printf(TEXT("HP Changed: Old = %.2f, Delta = %.2f, New = %.2f"), OldHP, Delta, CurrentHP);
        GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, DebugMsg);
    }

    if (CurrentHP <= 0.0f)
    {
        OnDeath.Broadcast();
    }
}


