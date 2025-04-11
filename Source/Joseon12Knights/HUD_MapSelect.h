#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUD_MapTile.h"
#include "GI_GameCoreInstance.h"
#include "HUD_MapSelect.generated.h"

UCLASS()
class JOSEON12KNIGHTS_API UHUD_MapSelect : public UUserWidget
{
    GENERATED_BODY()

public:

    UHUD_MapTile* SelectedTile = nullptr;

    UFUNCTION()
    void HandleTileSelected(UHUD_MapTile* NewTile);

    void ConfirmSelection(); // 엔터 처리

    UPROPERTY(meta = (BindWidget))
    UHUD_MapTile* Tile1;

    UPROPERTY(meta = (BindWidget))
    UHUD_MapTile* Tile2;

    UPROPERTY(meta = (BindWidget))
    UHUD_MapTile* Tile3;

protected:
    virtual void NativeConstruct() override;

    UFUNCTION()
    void OnBackClicked(); 

    UPROPERTY(meta = (BindWidget))
    class UButton* BackButton;




};
