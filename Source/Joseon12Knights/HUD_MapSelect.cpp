#include "HUD_MapSelect.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "PC_MenuController.h"

void UHUD_MapSelect::NativeConstruct()
{
    Super::NativeConstruct();

    if (APlayerController* PC = GetOwningPlayer())
    {
        FInputModeUIOnly InputMode;
        InputMode.SetWidgetToFocus(TakeWidget());
        InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
        PC->SetInputMode(InputMode);
        PC->bShowMouseCursor = true;
    }


    if (Tile3)
    {
        if (UGI_GameCoreInstance* GI = Cast<UGI_GameCoreInstance>(GetGameInstance()))
        {
            TArray<TSoftObjectPtr<UWorld>> Options;

            if (Tile1)
            {
                Options.Add(GI->SelectedPlayMode == EPlayMode::Single ? Tile1->SingleMap : Tile1->OnlineMap);
            }

            if (Tile2)
            {
                Options.Add(GI->SelectedPlayMode == EPlayMode::Single ? Tile2->SingleMap : Tile2->OnlineMap);
            }

            if (Options.Num() > 0)
            {
                int32 RandIndex = FMath::RandRange(0, Options.Num() - 1);
                if (GI->SelectedPlayMode == EPlayMode::Single)
                {
                    Tile3->SingleMap = Options[RandIndex];
                }
                else
                {
                    Tile3->OnlineMap = Options[RandIndex];
                }
            }
        }

    }

    if (BackButton)
    {
        BackButton->OnClicked.AddDynamic(this, &UHUD_MapSelect::OnBackClicked);
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
    UE_LOG(LogTemp, Warning, TEXT("HandleTileSelected(): %s"), *NewTile->GetName());

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

    if (UGI_GameCoreInstance* GI = Cast<UGI_GameCoreInstance>(GetGameInstance()))
    {
        TSoftObjectPtr<UWorld> MapToLoad;

        if (GI->SelectedPlayMode == EPlayMode::Single)
        {
            MapToLoad = SelectedTile->SingleMap;
        }
        else if (GI->SelectedPlayMode == EPlayMode::Online)
        {
            MapToLoad = SelectedTile->OnlineMap;
        }
        else
        {
            return;
        }

        FString MapName = MapToLoad.ToSoftObjectPath().GetAssetName();

        if (MapName.IsEmpty())
        {
            return;
        }

        GI->SelectedMap = MapToLoad;
        UGameplayStatics::OpenLevel(this, FName(*MapName));
    }
}







