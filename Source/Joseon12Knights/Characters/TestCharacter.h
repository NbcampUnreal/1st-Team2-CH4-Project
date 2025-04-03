

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
	
	void NormalAttack(const FInputActionValue& Value) override;

	void Skill(const FInputActionValue& Value) override;

	void Ultimate(const FInputActionValue& Value) override;

};
 