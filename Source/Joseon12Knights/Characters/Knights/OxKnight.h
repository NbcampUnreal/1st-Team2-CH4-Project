

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

};
