#include "HUD_CharacterSelectTile.h"
#include "HUD_CharacterSelectPanel.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Button.h"

void UHUD_CharacterSelectTile::NativeConstruct()
{
    Super::NativeConstruct();

    if (ClickButton)
    {
        ClickButton->OnClicked.AddDynamic(this, &UHUD_CharacterSelectTile::HandleClicked);
    }

    if (Port && PortraitTexture)
        Port->SetBrushFromTexture(PortraitTexture);
}

void UHUD_CharacterSelectTile::SetupCharacterTile(const FString& InCharacterID)
{
    CharacterID = InCharacterID;

    if (Name)
        Name->SetText(FText::FromString(CharacterID));
}

void UHUD_CharacterSelectTile::HandleClicked()
{
    UE_LOG(LogTemp, Warning, TEXT("Tile Clicked: %s"), *CharacterID);

    if (CharacterID.IsEmpty()) return;

    if (Panel1)
        Panel1->SetCharacter(CharacterID, TEXT("1"));
    else if (Panel2)
        Panel2->SetCharacter(CharacterID, TEXT("2"));
    else if (Panel3)
        Panel3->SetCharacter(CharacterID, TEXT("3"));
    else if (Panel4)
        Panel4->SetCharacter(CharacterID, TEXT("4"));

}

