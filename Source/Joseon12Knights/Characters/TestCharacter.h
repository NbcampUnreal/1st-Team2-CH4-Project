

#pragma once

#include "CoreMinimal.h"
#include "PlayerCharacter.h"
#include "TestCharacter.generated.h"

/**
 *
 */
UCLASS(Blueprintable)
class JOSEON12KNIGHTS_API ATestCharacter : public APlayerCharacter
{
	GENERATED_BODY()
protected:
	virtual void Skill(const FInputActionValue& Value) override;

	virtual void ServerSkill_Implementation() override;

	virtual void MulticastSkill_Implementation() override;

	virtual void Ultimate(const FInputActionValue& Value) override;

	virtual void ServerUltimate_Implementation() override;

	virtual void MulticastUltimate_Implementation() override;

};
