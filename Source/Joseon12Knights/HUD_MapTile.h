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

    // VS 모드일 때 사용할 맵
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map")
    TSoftObjectPtr<UWorld> SingleMap;

    // 온라인 모드일 때 사용할 맵
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map")
    TSoftObjectPtr<UWorld> OnlineMap;

    UFUNCTION()
    void OnTileClicked(); // 클릭 시 호출됨

    // 선택 시 상태 적용
    void SetSelected(bool bSelected);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
    USoundBase* SelectSound;

    // 타일 ID (선택적 정보)
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 TileID = 0;
};
