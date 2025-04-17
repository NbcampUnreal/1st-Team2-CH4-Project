#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "HUD_ReadyText.h"               
#include "Blueprint/UserWidget.h"  
#include "PC_LobbyController.generated.h"


class UHUD_Lobby; // 위젯 C++ 클래스
class UUserWidget;

UCLASS()
class JOSEON12KNIGHTS_API APC_LobbyController : public APlayerController
{
    GENERATED_BODY()

protected:
    virtual void BeginPlay() override;
    virtual void SetupInputComponent() override;
    virtual void Tick(float DeltaSeconds) override;

    void HandleEnterKey();      // Enter 키 입력 처리
    void SetLobbyCameraView();  // 고정 카메라로 시점 전환

    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UUserWidget> LobbyHUDClass;

    UPROPERTY()
    UHUD_ReadyText* LobbyHUD;  // 실제 인스턴스 보관
public:
    UFUNCTION(Server, Reliable)
    void Server_PressReady();

    UFUNCTION(Server, Reliable)
    void Server_AttemptStartMatch();

    UFUNCTION(Client, Reliable)
    void Client_SetSelectedCharacterID(const FString& InCharacterID);

    UFUNCTION(Client, Reliable)
    void Client_ShowReadyUI(int32 PlayerIndex);


};
