#pragma once

#include "CoreMinimal.h"
#include "PlayerCharacter.h"
#include "OxKnight.generated.h"

UCLASS(Blueprintable)
class JOSEON12KNIGHTS_API AOxKnight : public APlayerCharacter
{
	GENERATED_BODY()

protected:
	virtual void Skill(const FInputActionValue& Value) override;

	virtual void ServerSkill_Implementation() override;

	virtual void MulticastSkill_Implementation() override;

	virtual void Ultimate(const FInputActionValue& Value) override;

	virtual void ServerUltimate_Implementation() override;

	virtual void MulticastUltimate_Implementation() override;


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

	// 돌진 후 충돌 검사를 위한 타이머 핸들
	FTimerHandle UltimateCollisionTimerHandle;

	void PlayPersistentSkillMontage();

	void StopPersistentSkillAnimation();

	void PerformUltimateCollisionCheck();
	TSet<AActor*> ProcessedTargets;

// ===========
//  디버그 함수
// ===========
public:
	AOxKnight();
	UFUNCTION(BlueprintCallable, Category = "Debug")
	void DebugPrintSelfBuffs() const;

	UFUNCTION(BlueprintCallable, Category = "Debug")
	void DebugPrintTargetBuffs(APlayerCharacter* Target) const;
};
