#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PC_LobbyController.generated.h"

UCLASS()
class JOSEON12KNIGHTS_API APC_LobbyController : public APlayerController
{
    GENERATED_BODY()

protected:
    virtual void BeginPlay() override;
    virtual void SetupInputComponent() override;


    void HandleEnterKey();      // Enter 키 입력 처리
    void SetLobbyCameraView();  // 고정 카메라로 시점 전환

public:
    UFUNCTION(Server, Reliable)
    void Server_PressReady();

    UFUNCTION(Server, Reliable)
    void Server_AttemptStartMatch();  // 호스트 전용

};
