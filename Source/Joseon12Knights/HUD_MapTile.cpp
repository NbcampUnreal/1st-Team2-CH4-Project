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
    if (TileID == 3)
    {
        if (UHUD_MapSelect* Parent = GetTypedOuter<UHUD_MapSelect>())
        {
            if (UGI_GameCoreInstance* GI = Cast<UGI_GameCoreInstance>(GetGameInstance()))
            {
                TArray<TSoftObjectPtr<UWorld>> Options;

                if (GI->SelectedPlayMode == EPlayMode::Single)
                {
                    if (Parent->Tile1 && Parent->Tile1->SingleMap.IsValid())
                    {
                        Options.Add(Parent->Tile1->SingleMap);
                    }
                    if (Parent->Tile2 && Parent->Tile2->SingleMap.IsValid())
                    {
                        Options.Add(Parent->Tile2->SingleMap);
                    }

                    if (Options.Num() > 0)
                    {
                        int32 RandIndex = FMath::RandRange(0, Options.Num() - 1);
                        SingleMap = Options[RandIndex];
                    }
                }
                else if (GI->SelectedPlayMode == EPlayMode::Online)
                {
                    if (Parent->Tile1 && Parent->Tile1->OnlineMap.IsValid())
                    {
                        Options.Add(Parent->Tile1->OnlineMap);
                    }
                    if (Parent->Tile2 && Parent->Tile2->OnlineMap.IsValid())
                    {
                        Options.Add(Parent->Tile2->OnlineMap);
                    }

                    if (Options.Num() > 0)
                    {
                        int32 RandIndex = FMath::RandRange(0, Options.Num() - 1);
                        OnlineMap = Options[RandIndex];
                    }
                }
            }
        }
    }

    UE_LOG(LogTemp, Warning, TEXT("🟩 Tile 클릭됨: %s"), *GetName());

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

