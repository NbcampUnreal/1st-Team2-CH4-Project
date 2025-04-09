#pragma once

#include "CoreMinimal.h"
#include "Components/Image.h" 
#include "Animation/WidgetAnimation.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"
#include "HUD_CharacterSelectTile.generated.h"

class UHUD_CharacterSelectPanel;
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

    UPROPERTY(meta = (BindWidgetAnim), Transient)
    UWidgetAnimation* ShakeAnim;


    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
    UTexture2D* PortraitTexture;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character")
    TMap<FString, FString> CharacterNameMap;


    void UpdateSelectionIndicators(const TMap<FString, TArray<int32>>& Selections);

protected:
    FString CharacterID;

    UPROPERTY(meta = (BindWidget)) UImage* Port;
    UPROPERTY(meta = (BindWidget)) UTextBlock* Name;
    UPROPERTY(meta = (BindWidget)) UButton* ClickButton;

    UPROPERTY(meta = (BindWidget)) UImage* BorderP1;
    UPROPERTY(meta = (BindWidget)) UImage* BorderP2;
    UPROPERTY(meta = (BindWidget)) UImage* BorderP3;
    UPROPERTY(meta = (BindWidget)) UImage* BorderP4;


    void PlayShakeAnimation();

    UFUNCTION()
    void HandleClicked();
};
