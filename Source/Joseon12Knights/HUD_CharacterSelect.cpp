#include "HUD_CharacterSelect.h"
#include "HUD_CharacterSelectTile.h"
#include "HUD_CharacterSelectPanel.h"
#include "Components/Button.h"

void UHUD_CharacterSelect::NativeConstruct()
{
    Super::NativeConstruct();

    // 타일에 패널 주입
    TArray<UHUD_CharacterSelectTile*> Tiles = {
        CharacterPannel_1, CharacterPannel_2, CharacterPannel_3, CharacterPannel_4,
        CharacterPannel_5, CharacterPannel_6, CharacterPannel_7, CharacterPannel_8
    };

    for (UHUD_CharacterSelectTile* Tile : Tiles)
    {
        if (Tile)
        {
            Tile->Panel1 = Pannel1;
            Tile->Panel2 = Pannel2;
            Tile->Panel3 = Pannel3;
            Tile->Panel4 = Pannel4;
        }
    }

    if (CharacterPannel_1) CharacterPannel_1->SetupCharacterTile(TEXT("1"));
    if (CharacterPannel_2) CharacterPannel_2->SetupCharacterTile(TEXT("2"));
    if (CharacterPannel_3) CharacterPannel_3->SetupCharacterTile(TEXT("3"));
    if (CharacterPannel_4) CharacterPannel_4->SetupCharacterTile(TEXT("4"));
    if (CharacterPannel_5) CharacterPannel_5->SetupCharacterTile(TEXT("5"));
    if (CharacterPannel_6) CharacterPannel_6->SetupCharacterTile(TEXT("6"));
    if (CharacterPannel_7) CharacterPannel_7->SetupCharacterTile(TEXT("?"));
    if (CharacterPannel_8) CharacterPannel_8->SetupCharacterTile(TEXT("!"));

    if (BackButton)
        BackButton->OnClicked.AddDynamic(this, &UHUD_CharacterSelect::OnBackClicked);
}

void UHUD_CharacterSelect::OnBackClicked()
{
    RemoveFromParent();

    if (APlayerController* PC = GetOwningPlayer())
    {
        if (UClass* MainMenuClass = LoadClass<UUserWidget>(nullptr, TEXT("/Game/PlatformFighterKit/Blueprints/Hud/Menus/Main/HUD_MainMenu.HUD_MainMenu_C")))
        {
            if (UUserWidget* MainMenu = CreateWidget<UUserWidget>(PC, MainMenuClass))
            {
                MainMenu->AddToViewport();
            }
        }
    }
}