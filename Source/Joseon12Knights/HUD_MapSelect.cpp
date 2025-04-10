#include "HUD_MapSelect.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "PC_MenuController.h"

void UHUD_MapSelect::NativeConstruct()
{
    Super::NativeConstruct();

    if (Tile3)
    {
        TArray<TSoftObjectPtr<UWorld>> Options;
        if (Tile1 && Tile1->LevelToLoad.IsValid())
        {
            Options.Add(Tile1->LevelToLoad);
        }
        if (Tile2 && Tile2->LevelToLoad.IsValid())
        {
            Options.Add(Tile2->LevelToLoad);
        }

        if (Options.Num() > 0)
        {
            int32 RandIndex = FMath::RandRange(0, Options.Num() - 1);
            Tile3->LevelToLoad = Options[RandIndex];
        }
    }



}

void UHUD_MapSelect::OnBackClicked()
{
    if (APlayerController* PC = GetOwningPlayer())
    {
        if (APC_MenuController* MenuPC = Cast<APC_MenuController>(PC))
        {
            MenuPC->HandleBackToCharacterSelect(); 
        }
    }
}

void UHUD_MapSelect::HandleTileSelected(UHUD_MapTile* NewTile)
{
    if (SelectedTile && SelectedTile != NewTile)
    {
        SelectedTile->SetSelected(false);
    }

    SelectedTile = NewTile;
    if (SelectedTile)
    {
        SelectedTile->SetSelected(true);
    }

}

void UHUD_MapSelect::ConfirmSelection()
{
    if (!SelectedTile)
    {
        return;
    }

    if (!SelectedTile->LevelToLoad.IsValid())
    {
        return;
    }

    UGameplayStatics::OpenLevel(this, FName(*SelectedTile->LevelToLoad.ToSoftObjectPath().GetAssetName()));
}


