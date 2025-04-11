#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BuffComponent.generated.h"

UENUM(BlueprintType)
enum class EBuffType : uint8
{
	None,
	Slow,
	Poison,
	Bleed,
	Stealth,
	AttackBuff,
	DefenseBuff
};

USTRUCT(BlueprintType)
struct FBuffInfo
{
	GENERATED_BODY()

	// 버프 종류
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Buff")
	EBuffType BuffType;

	// 버프 지속시간 (초)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Buff")
	float Duration;

	// 공격력 배율 (예: 1.2면 20% 증가, 0.8면 20% 감소)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Buff")
	float AttackMultiplier;

	// 방어력 배율 (예: 0.6이면 40% 피해 감소)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Buff")
	float DefenseMultiplier;

	// 이동속도 배율 (예: 0.7이면 30% 느려짐)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Buff")
	float MoveSpeedMultiplier;

	// 도트 데미지: 공격력 대비 비율(초당)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Buff")
	float DamageOverTimePercent;

	// 공격자를 기준으로 한 공격력 (도트 데미지 계산용)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Buff")
	float SourceAttackValue;

	// 내부 타이머: 경과 시간
	float ElapsedTime;

	FBuffInfo()
		: BuffType(EBuffType::None)
		, Duration(0.f)
		, AttackMultiplier(1.f)
		, DefenseMultiplier(1.f)
		, MoveSpeedMultiplier(1.f)
		, DamageOverTimePercent(0.f)
		, SourceAttackValue(0.f)
		, ElapsedTime(0.f)
	{}
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class JOSEON12KNIGHTS_API UBuffComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UBuffComponent();

	// 활성 버프 목록
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Buff")
	TArray<FBuffInfo> ActiveBuffs;

	// 버프 추가 함수
	UFUNCTION(BlueprintCallable, Category = "Buff")
	void AddBuff(const FBuffInfo& NewBuff);

	// 특정 종류의 버프 제거
	UFUNCTION(BlueprintCallable, Category = "Buff")
	void RemoveBuff(EBuffType BuffType);

	// 공격력 배율 계산
	UFUNCTION(BlueprintCallable, Category = "Buff")
	float GetAttackMultiplier() const;

	// 방어력 배율 계산
	UFUNCTION(BlueprintCallable, Category = "Buff")
	float GetDefenseMultiplier() const;

	// 이동속도 배율 계산
	UFUNCTION(BlueprintCallable, Category = "Buff")
	float GetMoveSpeedMultiplier() const;

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

private:
	// 매 프레임 버프 업데이트: 경과시간 증가 후, 만료된 버프 제거
	void UpdateBuffs(float DeltaTime);
};
