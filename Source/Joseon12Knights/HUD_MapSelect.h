#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUD_MapSelect.generated.h"

UCLASS()
class JOSEON12KNIGHTS_API UHUD_MapSelect : public UUserWidget
{
    GENERATED_BODY()

protected:
    virtual void NativeConstruct() override;

    UFUNCTION()
    void OnBackClicked(); 

    UPROPERTY(meta = (BindWidget))
    class UButton* BackButton;
};
