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

	// ProceedToMatch 오버라이드: 싱글은 OpenLevel
	virtual void ProceedToMatch() override;
};