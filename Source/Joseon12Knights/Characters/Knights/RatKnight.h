

#pragma once

#include "CoreMinimal.h"
#include "PlayerCharacter.h"
#include "RatKnight.generated.h"

UCLASS(Blueprintable)
class JOSEON12KNIGHTS_API ARatKnight : public APlayerCharacter
{
	GENERATED_BODY()

protected:
	
	// WŰ ��ų : �㱸��
	void Skill(const FInputActionValue& Value) override;
	// RŰ �ñر� : 1�� ��Ÿ
	void Ultimate(const FInputActionValue& Value) override;

	// W ��ų ����

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RatKnight Skill")
	UParticleSystem* RatSkill1Effect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RatKnight Skill")
	USoundBase* RatSkill1Sound;

	// R �ñر� ����

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RatKnight Skill")
	UParticleSystem* RatUltimateEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RatKnight Skill")
	USoundBase* RatUltimateSound;

private:
	// ��Ÿ�� ���� ����
	bool bCanUseSkill = true;
	bool bCanUseUltimate = true;
	FTimerHandle SkillCooldownTimerHandle;
	FTimerHandle UltimateCooldownTimerHandle;
};