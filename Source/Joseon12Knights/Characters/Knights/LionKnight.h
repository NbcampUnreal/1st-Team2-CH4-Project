#pragma once

#include "CoreMinimal.h"
#include "PlayerCharacter.h"
#include "LionKnight.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class JOSEON12KNIGHTS_API ALionKnight : public APlayerCharacter
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TigerKnight Skill")
	UParticleSystem* TigerSkill1Effect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TigerKnight Skill")
	USoundBase* TigerSkill1Sound;

	// R 궁극기 에셋
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TigerKnight Skill")
	UParticleSystem* TigerUltimateEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TigerKnight Skill")
	USoundBase* TigerUltimateSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TigerKnight Skill")
	UNiagaraSystem* TigerBleedNiagaraEffect;

private:
	// 쿨타임 관리 변수
	bool bCanUseSkill = true;
	bool bCanUseUltimate = true;
	FTimerHandle SkillCooldownTimerHandle;
	FTimerHandle UltimateCooldownTimerHandle;


	// ==========
	//  디버그 함수
	// ==========
public:
	UFUNCTION(BlueprintCallable, Category = "Debug")
	void DebugPrintSelfBuffs() const;
	
};
