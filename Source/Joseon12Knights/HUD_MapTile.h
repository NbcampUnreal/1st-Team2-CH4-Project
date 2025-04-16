// HUD_MapTile.h

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h" 
#include "HUD_MapTile.generated.h"

UCLASS()
class JOSEON12KNIGHTS_API UHUD_MapTile : public UUserWidget
{
    GENERATED_BODY()

public:
    virtual void NativeConstruct() override;

    bool bIsSelected = false;

    UPROPERTY(meta = (BindWidget))
    class UButton* Button_134;

    UPROPERTY(meta = (BindWidget))
    class UImage* Border_Selection;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map")
    TSoftObjectPtr<UWorld> SingleMap;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map")
    TSoftObjectPtr<UWorld> OnlineMap;

    UFUNCTION()
    void OnTileClicked(); 

    void SetSelected(bool bSelected);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
    USoundBase* SelectSound;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* MapText;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map")
    FText MapLabel;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 TileID = 0;

    UPROPERTY(meta = (BindWidget))
    UImage* MapIcon;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map")
    UTexture2D* TileImage;


};
