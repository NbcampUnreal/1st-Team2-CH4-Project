// GM_MainMode.h
#pragma once

#include "CoreMinimal.h"
#include "GM_BaseMode.h"
#include "GS_FighterState.h"
#include "GM_MainMode.generated.h"

UCLASS()
class JOSEON12KNIGHTS_API AGM_MainMode : public AGM_BaseMode
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
};
