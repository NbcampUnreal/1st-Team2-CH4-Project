#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUD_OnlineMenus.generated.h"

UCLASS()
class JOSEON12KNIGHTS_API UHUD_OnlineMenus : public UUserWidget
{
    GENERATED_BODY()

protected:
    virtual void NativeConstruct() override;

    UFUNCTION()
    void OnBackClicked(); 

    UFUNCTION()
    void OnHostClicked();


    UPROPERTY(meta = (BindWidget))
    class UButton* HostButton;

    UPROPERTY(meta = (BindWidget))
    class UButton* JoinButton;

    UPROPERTY(meta = (BindWidget))
    class UButton* Back;
};
