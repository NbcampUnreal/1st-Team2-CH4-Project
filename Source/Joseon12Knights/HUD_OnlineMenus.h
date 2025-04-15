#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PC_MenuController.h" 
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
    void OnClickJoin();

    UFUNCTION()
    void OnHostClicked();

    UFUNCTION()
    void OnCreateSessionConfirmed();

    UPROPERTY(meta = (BindWidget))
    class UButton* HostButton;

    UPROPERTY(meta = (BindWidget))
    class UButton* JoinButton;

    UPROPERTY(meta = (BindWidget))
    class UButton* Back;

    UPROPERTY(meta = (BindWidget))
    class UButton* CreateSessionButton;

    UPROPERTY(meta = (BindWidget))
    class UEditableTextBox* SessionName;

    UPROPERTY(meta = (BindWidget))
    class UVerticalBox* SessionNamePanel;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Maps")
    TSoftObjectPtr<UWorld> LobbyMap;
};
