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

	// W ��ų ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TigerKnight Skill")
	UParticleSystem* TigerSkill1Effect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TigerKnight Skill")
	USoundBase* TigerSkill1Sound;

	// R �ñر� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TigerKnight Skill")
	UParticleSystem* TigerUltimateEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TigerKnight Skill")
	USoundBase* TigerUltimateSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TigerKnight Skill")
	UNiagaraSystem* TigerBleedNiagaraEffect;

private:
	// ��Ÿ�� ���� ����
	bool bCanUseSkill = true;
	bool bCanUseUltimate = true;
	FTimerHandle SkillCooldownTimerHandle;
	FTimerHandle UltimateCooldownTimerHandle;


	// ==========
	//  ����� �Լ�
	// ==========
public:
	UFUNCTION(BlueprintCallable, Category = "Debug")
	void DebugPrintSelfBuffs() const;
	
};
