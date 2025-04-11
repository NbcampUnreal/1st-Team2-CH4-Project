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
	void DebugPrintBuffs() const;
};
