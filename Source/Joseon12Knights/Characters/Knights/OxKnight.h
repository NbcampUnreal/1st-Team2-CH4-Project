#pragma once

#include "CoreMinimal.h"
#include "PlayerCharacter.h"
#include "OxKnight.generated.h"

UCLASS(Blueprintable)
class JOSEON12KNIGHTS_API AOxKnight : public APlayerCharacter
{
	GENERATED_BODY()

protected:
	void Skill(const FInputActionValue& Value) override;
	void Ultimate(const FInputActionValue& Value) override;

	// W 스킬 에셋
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OxKnight Skill")
	UParticleSystem* OxSkill1Effect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OxKnight Skill")
	USoundBase* OxSkill1Sound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OxKnight Skill")
	UAnimMontage* PersistentSkillMontage;

	// R 궁극기 에셋
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OxKnight Skill")
	UParticleSystem* OxUltimateEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OxKnight Skill")
	USoundBase* OxUltimateSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OxKnight Skill")
	float UltimateLaunchForce = 1000.0f;

private:
	// 쿨타임 관리 변수
	bool bCanUseSkill = true;
	bool bCanUseUltimate = true;
	FTimerHandle SkillCooldownTimerHandle;
	FTimerHandle UltimateCooldownTimerHandle;

	FTimerHandle PlayPersistentMontageTimerHandle;
	FTimerHandle StopAnimationTimerHandle;

	void PlayPersistentSkillMontage();

	UFUNCTION()
	void StopPersistentSkillAnimation();

// ===========
//  디버그 함수
// ===========
public:
	UFUNCTION(BlueprintCallable, Category = "Debug")
	void DebugPrintSelfBuffs() const;

	UFUNCTION(BlueprintCallable, Category = "Debug")
	void DebugPrintTargetBuffs(APlayerCharacter* Target) const;
};
