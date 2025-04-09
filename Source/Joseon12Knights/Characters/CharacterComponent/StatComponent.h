// 

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CharacterStatInfo.h"
#include "StatComponent.generated.h"

// 각 캐릭터의 HP, 공격력, 방어력 등 스탯을 관리하는 컴포넌트
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class JOSEON12KNIGHTS_API UStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UStatComponent();

protected:
	// 초기화된 스탯 정보(최대 체력, 공격력, 방어력 등)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stat")
	FCharacterStatInfo BaseStats;

	// 현재 HP
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Stat")
	float CurrentHP;

public:
	// DataTable에서 로드하거나, 직접 스탯 세팅
	UFUNCTION(BlueprintCallable, Category="Stat")
	void InitializeStats(const FCharacterStatInfo& NewStats);

	// 현재 HP
	UFUNCTION(BlueprintCallable, Category="Stat")
	float GetCurrentHP() const { return CurrentHP; }

	// 최대 HP
	UFUNCTION(BlueprintCallable, Category="Stat")
	float GetMaxHP() const { return BaseStats.MaxHP; }

	// 공격력, 방어력 Getter
	UFUNCTION(BlueprintCallable, Category="Stat")
	float GetAttack() const { return BaseStats.Attack; }

	UFUNCTION(BlueprintCallable, Category="Stat")
	float GetDefense() const { return BaseStats.Defense; }

	// HP 변경 (데미지/회복)
	UFUNCTION(BlueprintCallable, Category="Stat")
	void ChangeHP(float Delta);

	// 사망 처리 등 이벤트가 필요하면 Delegate 사용 가능
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeath);
	UPROPERTY(BlueprintAssignable, Category="Stat")
	FOnDeath OnDeath;

protected:
	virtual void BeginPlay() override;
};
