

#pragma once

#include "CoreMinimal.h"
#include "PlayerCharacter.h"
#include "RatKnight.generated.h"

UCLASS(Blueprintable)
class JOSEON12KNIGHTS_API ARatKnight : public APlayerCharacter
{
	GENERATED_BODY()

protected:
	
	// WÅ° ½ºÅ³ : Áã±¸¸Û
	void Skill(const FInputActionValue& Value) override;
	// RÅ° ±Ã±Ø±â : 1µî ³­Å¸
	void Ultimate(const FInputActionValue& Value) override;

	// W ½ºÅ³ ¿¡¼Â

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RatKnight Skill")
	UParticleSystem* RatSkill1Effect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RatKnight Skill")
	USoundBase* RatSkill1Sound;

	// R ±Ã±Ø±â ¿¡¼Â

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RatKnight Skill")
	UParticleSystem* RatUltimateEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RatKnight Skill")
	USoundBase* RatUltimateSound;

private:
	// ÄðÅ¸ÀÓ °ü¸® º¯¼ö
	bool bCanUseSkill = true;
	bool bCanUseUltimate = true;
	FTimerHandle SkillCooldownTimerHandle;
	FTimerHandle UltimateCooldownTimerHandle;
};