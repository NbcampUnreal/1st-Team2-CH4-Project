// HUD_MapTile.cpp

#include "HUD_MapTile.h"
#include "HUD_MapSelect.h"
#include "Components/Button.h"
#include "Components/Border.h"

void UHUD_MapTile::NativeConstruct()
{
    Super::NativeConstruct();

    if (Button_134)
    {
        Button_134->OnClicked.AddDynamic(this, &UHUD_MapTile::OnTileClicked);
    }

    // 초기 보더 숨김
    if (Border_Selection)
    {
        Border_Selection->SetVisibility(ESlateVisibility::Hidden);
    }
}


void UHUD_MapTile::OnTileClicked()
{
    // 3번 타일이면 HUD_MapSelect에서 Tile1/Tile2의 LevelToLoad를 참조
    if (TileID == 3)
    {
        if (UHUD_MapSelect* Parent = GetTypedOuter<UHUD_MapSelect>())
        {
            TArray<TSoftObjectPtr<UWorld>> Options;

            if (Parent->Tile1 && Parent->Tile1->LevelToLoad.IsValid())
            {
                Options.Add(Parent->Tile1->LevelToLoad);
            }

            if (Parent->Tile2 && Parent->Tile2->LevelToLoad.IsValid())
            {
                Options.Add(Parent->Tile2->LevelToLoad);
            }

            if (Options.Num() > 0)
            {
                int32 RandIndex = FMath::RandRange(0, Options.Num() - 1);
                LevelToLoad = Options[RandIndex];
            }
        }
    }

    if (UHUD_MapSelect* Parent = GetTypedOuter<UHUD_MapSelect>())
    {
        Parent->HandleTileSelected(this);
    }
}





void UHUD_MapTile::SetSelected(bool bSelected)
{
    bIsSelected = bSelected;

    if (Border_Selection)
    {
        Border_Selection->SetVisibility(bSelected ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    }
}

