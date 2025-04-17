#pragma once

#include "CoreMinimal.h"
#include "PlayerCharacter.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "RatKnight.generated.h"

// 자 기사의 스킬은 모두 공격형, 디버프 효과로 적용됨.
// 1) W : 쥐구멍 – 슬로우 디버프 & 은신
// 2) R : 1등 난타 – 독 디버프

UCLASS(Blueprintable)
class JOSEON12KNIGHTS_API ARatKnight : public APlayerCharacter
{
	GENERATED_BODY()

protected:
	virtual void Skill(const FInputActionValue& Value) override;

	virtual void ServerSkill_Implementation() override;

	virtual void MulticastSkill_Implementation() override;

	virtual void Ultimate(const FInputActionValue& Value) override;

	virtual void ServerUltimate_Implementation() override;

	virtual void MulticastUltimate_Implementation() override;

protected:
	// ===========
	// W Skill Asset
	// ===========
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RatKnight Skill")
	UParticleSystem* RatSkill1Effect;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RatKnight Skill")
	USoundBase* RatSkill1Sound;

	// 은신 관련 효과
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RatKnight Skill")
	UParticleSystem* StealthEndEffect;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RatKnight Skill")
	USoundBase* StealthEndSound;

	// 은신 머티리얼 관련
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RatKnight Stealth")
	TArray<UMaterialInterface*> OriginalMaterials;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RatKnight Stealth")
	TArray<UMaterialInterface*> StealthMaterials;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RatKnight Stealth")
	TArray<UMaterialInterface*> OriginalWeaponMaterials;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RatKnight Stealth")
	TArray<UMaterialInterface*> OriginalShieldMaterials;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RatKnight Stealth")
	TArray<UMaterialInterface*> StealthWeaponMaterials;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RatKnight Stealth")
	TArray<UMaterialInterface*> StealthShieldMaterials;

	// ===========
	// R Skill Asset
	// ===========
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RatKnight Skill")
	UParticleSystem* RatUltimateEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RatKnight Skill")
	USoundBase* RatUltimateSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RatKnight Skill")
	UNiagaraSystem* RatPoisonNiagaraEffect;

public:
	virtual void BeginPlay() override;

	// 은신 관련 함수
	void SetStealthVisibility(bool bIsStealthed);
	void EndStealth();

private:
	// 쿨타임 관리 변수
	bool bCanUseSkill = true;
	bool bCanUseUltimate = true;
	FTimerHandle SkillCooldownTimerHandle;
	FTimerHandle UltimateCooldownTimerHandle;

	// 은신 관련 변수
	bool bIsInStealthMode = false;
	FTimerHandle StealthTimerHandle;
	float StealthDuration = 5.0f;

// ===========
//  디버그 함수
// ===========
public:
	UFUNCTION(BlueprintCallable, Category = "Debug")
	void DebugPrintTargetBuffs(APlayerCharacter* Target) const;
};