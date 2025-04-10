#include "HUD_CharacterSelectPanel.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UHUD_CharacterSelectPanel::SetCharacter(const FString& CharacterID, const FString& PlayerID)
{
    if (Name)
        Name->SetText(FText::FromString(CharacterID));

    if (Player)
        Player->SetText(FText::FromString(TEXT("Player ") + PlayerID));

    if (!CharacterImage)
    {
        UE_LOG(LogTemp, Error, TEXT(" CharacterImage is nullptr"));
        return;
    }

    if (CharacterIconMap.Contains(CharacterID))
    {
        UTexture2D* FoundTexture = CharacterIconMap[CharacterID];

        if (CharacterImage && FoundTexture)
        {
            FSlateBrush Brush;
            Brush.SetResourceObject(FoundTexture);
            Brush.ImageSize = FVector2D(256.f, 256.f); 
            CharacterImage->SetBrush(Brush);
            UE_LOG(LogTemp, Warning, TEXT("Force-set image size for ID: %s"), *CharacterID);
        }
    }

    else
    {
        UE_LOG(LogTemp, Error, TEXT(" No texture found in map for CharacterID: %s"), *CharacterID);
    }
}
