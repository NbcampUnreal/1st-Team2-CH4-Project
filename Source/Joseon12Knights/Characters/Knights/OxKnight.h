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

	// W ��ų ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OxKnight Skill")
	UParticleSystem* OxSkill1Effect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OxKnight Skill")
	USoundBase* OxSkill1Sound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OxKnight Skill")
	UAnimMontage* PersistentSkillMontage;

	// R �ñر� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OxKnight Skill")
	UParticleSystem* OxUltimateEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OxKnight Skill")
	USoundBase* OxUltimateSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OxKnight Skill")
	float UltimateLaunchForce = 1000.0f;

private:
	// ��Ÿ�� ���� ����
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
//  ����� �Լ�
// ===========
public:
	UFUNCTION(BlueprintCallable, Category = "Debug")
	void DebugPrintSelfBuffs() const;

	UFUNCTION(BlueprintCallable, Category = "Debug")
	void DebugPrintTargetBuffs(APlayerCharacter* Target) const;
};
