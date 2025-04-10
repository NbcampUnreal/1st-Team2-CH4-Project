#pragma once

#include "CoreMinimal.h"
#include "PlayerCharacter.h"
#include "TigerKnight.generated.h"
UCLASS(Blueprintable)
class JOSEON12KNIGHTS_API ATigerKnight : public APlayerCharacter
{
	GENERATED_BODY()

protected:
	void Skill(const FInputActionValue& Value) override;
	void Ultimate(const FInputActionValue& Value) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TigerKnight Skill")
	UParticleSystem* TigerSkill1Effect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TigerKnight Skill")
	USoundBase* TigerSkill1Sound;

	// R ±Ã±Ø±â ¿¡¼Â

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TigerKnight Skill")
	UParticleSystem* TigerUltimateEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TigerKnight Skill")
	USoundBase* TigerUltimateSound;

private:
	// ÄðÅ¸ÀÓ °ü¸® º¯¼ö
	bool bCanUseSkill = true;
	bool bCanUseUltimate = true;
	FTimerHandle SkillCooldownTimerHandle;
	FTimerHandle UltimateCooldownTimerHandle;

};
