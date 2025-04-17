// UHUD_Lobby.h

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "HUD_ReadyText.generated.h"

UCLASS()
class JOSEON12KNIGHTS_API UHUD_ReadyText : public UUserWidget
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable)
    void ShowReadyForPlayer(int32 Index);

protected:
    virtual void NativeConstruct() override;



    UPROPERTY(meta = (BindWidget))
    class UTextBlock* ready_1;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* ready_2;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* ready_3;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* ready_4;

};
