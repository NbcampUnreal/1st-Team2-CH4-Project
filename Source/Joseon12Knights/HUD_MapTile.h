// HUD_MapTile.h

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h" 
#include "HUD_MapTile.generated.h"

UCLASS()
class JOSEON12KNIGHTS_API UHUD_MapTile : public UUserWidget
{
    GENERATED_BODY()

public:
    virtual void NativeConstruct() override;

    // 선택됐는지 여부
    bool bIsSelected = false;

    // 바인딩된 버튼, 보더
    UPROPERTY(meta = (BindWidget))
    class UButton* Button_134;

    UPROPERTY(meta = (BindWidget))
    class UImage* Border_Selection;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSoftObjectPtr<UWorld> LevelToLoad;



    UFUNCTION()
    void OnTileClicked(); // 이미 있음

    // 선택 시 상태 적용
    void SetSelected(bool bSelected);

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 TileID = 0;

};
