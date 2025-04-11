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

	// R �ñر� ����

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TigerKnight Skill")
	UParticleSystem* TigerUltimateEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TigerKnight Skill")
	USoundBase* TigerUltimateSound;



private:
	// ��Ÿ�� ���� ����
	bool bCanUseSkill = true;
	bool bCanUseUltimate = true;
	FTimerHandle SkillCooldownTimerHandle;
	FTimerHandle UltimateCooldownTimerHandle;

	//APlayerCharacter* GetTargetPlayer();
	//TArray<APlayerCharacter*> FindTargetsInRadius(const FVector& Origin, float Radius);
	//APlayerCharacter* SelectBestTarget(const TArray<APlayerCharacter*>& PotentialTargets);
	//

	//UPROPERTY(EditAnywhere, Category = "Targeting")
	//float AttackRadius = 106.0f;  // ���� ���� �ݰ�

	//UPROPERTY(EditAnywhere, Category = "Targeting")
	//float ForwardOffset = 30.0f; // ���� ������ �Ÿ�

	//UPROPERTY(EditAnywhere, Category = "Targeting")
	//bool bDebugTargeting = false; // ����� �ð�ȭ ����

	//UPROPERTY(EditAnywhere, Category = "Targeting")
	//TEnumAsByte<ECollisionChannel> TargetCollisionChannel = ECC_Pawn; // Ÿ�� ���� ä��
};
