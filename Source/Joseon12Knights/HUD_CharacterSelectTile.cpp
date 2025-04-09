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

    static TMap<FString, FString> CharacterNameMap = {
        {TEXT("1"), TEXT("자 기사")},
        {TEXT("2"), TEXT("축 기사")},
        {TEXT("3"), TEXT("인 기사")},
        {TEXT("4"), TEXT("진 기사")},
        {TEXT("5"), TEXT("잠금")},
        {TEXT("6"), TEXT("잠금2")},
        {TEXT("7"), TEXT("잠금3")},
        {TEXT("8"), TEXT("잠금4")}
    };

    FString* DisplayName = CharacterNameMap.Find(InCharacterID);
    FString FinalName = DisplayName ? *DisplayName : InCharacterID;

    if (Name)
        Name->SetText(FText::FromString(FinalName));
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

