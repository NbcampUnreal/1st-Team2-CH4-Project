#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUD_CharacterSelectTile.generated.h"

class UHUD_CharacterSelectPanel;
class UImage;
class UTextBlock;
class UButton;

UCLASS()
class JOSEON12KNIGHTS_API UHUD_CharacterSelectTile : public UUserWidget
{
    GENERATED_BODY()

public:
    virtual void NativeConstruct() override;

    UFUNCTION(BlueprintCallable)
    void SetupCharacterTile(const FString& InCharacterID);

    UPROPERTY()
    UHUD_CharacterSelectPanel* Panel1;

    UPROPERTY()
    UHUD_CharacterSelectPanel* Panel2;

    UPROPERTY()
    UHUD_CharacterSelectPanel* Panel3;

    UPROPERTY()
    UHUD_CharacterSelectPanel* Panel4;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
    UTexture2D* PortraitTexture;

protected:
    FString CharacterID;

    UPROPERTY(meta = (BindWidget)) UImage* Port;
    UPROPERTY(meta = (BindWidget)) UTextBlock* Name;
    UPROPERTY(meta = (BindWidget)) UButton* ClickButton;

    UFUNCTION()
    void HandleClicked();
};
