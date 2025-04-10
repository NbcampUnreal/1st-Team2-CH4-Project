

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OXKnight Skill")
	UParticleSystem* OXSkill1Effect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OXKnight Skill")
	USoundBase* OXSkill1Sound;

	// R ±Ã±Ø±â ¿¡¼Â

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OXKnight Skill")
	UParticleSystem* OXUltimateEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OXKnight Skill")
	USoundBase* OXUltimateSound;

private:
	// ÄðÅ¸ÀÓ °ü¸® º¯¼ö
	bool bCanUseSkill = true;
	bool bCanUseUltimate = true;
	FTimerHandle SkillCooldownTimerHandle;
	FTimerHandle UltimateCooldownTimerHandle;

};
