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

	//APlayerCharacter* GetTargetPlayer();
	//TArray<APlayerCharacter*> FindTargetsInRadius(const FVector& Origin, float Radius);
	//APlayerCharacter* SelectBestTarget(const TArray<APlayerCharacter*>& PotentialTargets);
	//

	//UPROPERTY(EditAnywhere, Category = "Targeting")
	//float AttackRadius = 106.0f;  // 공격 감지 반경

	//UPROPERTY(EditAnywhere, Category = "Targeting")
	//float ForwardOffset = 30.0f; // 전방 오프셋 거리

	//UPROPERTY(EditAnywhere, Category = "Targeting")
	//bool bDebugTargeting = false; // 디버그 시각화 여부

	//UPROPERTY(EditAnywhere, Category = "Targeting")
	//TEnumAsByte<ECollisionChannel> TargetCollisionChannel = ECC_Pawn; // 타겟 감지 채널
};
