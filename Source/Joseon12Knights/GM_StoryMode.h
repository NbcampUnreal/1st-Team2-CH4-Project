#pragma once

#include "CoreMinimal.h"
#include "GM_BaseMode.h"
#include "Characters/MainPlayerState.h"
#include "GM_StoryMode.generated.h"

UCLASS()
class JOSEON12KNIGHTS_API AGM_StoryMode : public AGM_BaseMode
{
    GENERATED_BODY()

public:
    virtual void HandlePlayerRespawn(AActor* PlayerActor) override;
    virtual bool CanRespawn(AActor* PlayerActor) const override;

protected:
    virtual void BeginPlay() override;
    virtual void ProceedToMatch() override;

};
