#pragma once

#include "CoreMinimal.h"
#include "GM_BaseMode.h"
#include "GM_LobbyMode.generated.h"

class APlayerStart;

UCLASS()
class JOSEON12KNIGHTS_API AGM_LobbyMode : public AGM_BaseMode
{
    GENERATED_BODY()

public:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaSeconds) override;
    virtual void PostLogin(APlayerController* NewPlayer) override;
    virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

    UFUNCTION(Server, Reliable)
    void Server_SetReady(APlayerController* PC);

    UFUNCTION(BlueprintCallable)
    void StartMatch();

protected:
    UPROPERTY()
    class UGI_GameCoreInstance* GameInstance;

    UPROPERTY()
    TMap<AController*, int32> PlayerNumberMap; // Controller → PlayerIndex

    bool bStartClosed = false;
};
