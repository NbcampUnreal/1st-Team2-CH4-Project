#pragma once

#include "CoreMinimal.h"
#include "GM_BaseMode.h"
#include "GM_SingleMode.generated.h"

UCLASS()
class JOSEON12KNIGHTS_API AGM_SingleMode : public AGM_BaseMode
{
	GENERATED_BODY()

public:
	AGM_SingleMode();

	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	TMap<FString, TSubclassOf<APawn>> CharacterBPMap;
};
