// GM_MatchMode.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GM_MatchMode.generated.h"

UCLASS()
class JOSEON12KNIGHTS_API AGM_MatchMode : public AGameModeBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	void SpawnCharacters();

};
