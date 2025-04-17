#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "RespawnManagerInterface.generated.h"

UINTERFACE(MinimalAPI)
class URespawnManagerInterface : public UInterface
{
    GENERATED_BODY()
};

class IRespawnManagerInterface
{
    GENERATED_BODY()

public:
    virtual void HandlePlayerRespawn(AActor* PlayerActor) = 0;
};

