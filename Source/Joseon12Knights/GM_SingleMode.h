// GM_SingleMode.h
#pragma once

#include "CoreMinimal.h"
#include "GM_BaseMode.h"
#include "GM_SingleMode.generated.h"

UCLASS()
class JOSEON12KNIGHTS_API AGM_SingleMode : public AGM_BaseMode
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	virtual void ProceedToMatch() override;
};