#include "HUD_CharacterSelectTile.h"
#include "HUD_CharacterSelectPanel.h"
#include "HUD_CharacterSelect.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "PC_MenuController.h"

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
    CharacterNameMap = 
    {
        {TEXT("1"), TEXT("자 기사")},
        {TEXT("2"), TEXT("축 기사")},
        {TEXT("3"), TEXT("인 기사")},
        {TEXT("4"), TEXT("진 기사")},
        {TEXT("5"), TEXT("잠금")},
        {TEXT("6"), TEXT("잠금")},
        {TEXT("7"), TEXT("잠금")},
        {TEXT("8"), TEXT("잠금")}
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

    if (CharacterID == "5" || CharacterID == "6" || CharacterID == "7" || CharacterID == "8")
    {
        StartShake();
        return;
    }

    FString FinalCharacterName = CharacterNameMap.Contains(CharacterID)
        ? CharacterNameMap[CharacterID]
        : CharacterID;

        int32 ControllerId = 0;
        if (APlayerController* PC = GetOwningPlayer())
        {
            if (ULocalPlayer* LP = PC->GetLocalPlayer())
            {
                ControllerId = LP->GetControllerId();
            }
        }

        FString PlayerIDText = FString::Printf(TEXT("%d"), ControllerId + 1);

        switch (ControllerId)
        {
        case 0: if (Panel1) Panel1->SetCharacter(FinalCharacterName, PlayerIDText); break;
        case 1: if (Panel2) Panel2->SetCharacter(FinalCharacterName, PlayerIDText); break;
        case 2: if (Panel3) Panel3->SetCharacter(FinalCharacterName, PlayerIDText); break;
        case 3: if (Panel4) Panel4->SetCharacter(FinalCharacterName, PlayerIDText); break;
        }

        if (UHUD_CharacterSelect* SelectHUD = GetTypedOuter<UHUD_CharacterSelect>())
        {
            SelectHUD->NotifyCharacterSelected(CharacterID, ControllerId);
        }
        else if (UHUD_CharacterStory* StoryHUD = GetTypedOuter<UHUD_CharacterStory>())
        {
            StoryHUD->NotifyCharacterSelected(CharacterID, ControllerId);
        }

        if (APlayerController* PC = GetOwningPlayer())
        {
            if (APC_MenuController* MenuPC = Cast<APC_MenuController>(PC))
            {
                MenuPC->SelectCharacter(CharacterID);
            }
        }
}





void UHUD_CharacterSelectTile::UpdateSelectionIndicators(const TMap<FString, TArray<int32>>& Selections)
{
    if (BorderP1) BorderP1->SetVisibility(ESlateVisibility::Hidden);
    if (BorderP2) BorderP2->SetVisibility(ESlateVisibility::Hidden);
    if (BorderP3) BorderP3->SetVisibility(ESlateVisibility::Hidden);
    if (BorderP4) BorderP4->SetVisibility(ESlateVisibility::Hidden);

    if (const TArray<int32>* Players = Selections.Find(CharacterID))
    {
        for (int32 Index : *Players)
        {
            switch (Index)
            {
            case 0:
                if (BorderP1)
                    BorderP1->SetVisibility(ESlateVisibility::Visible);
                break;
            case 1:
                if (BorderP2)
                    BorderP2->SetVisibility(ESlateVisibility::Visible);
                break;
            case 2:
                if (BorderP3)
                    BorderP3->SetVisibility(ESlateVisibility::Visible);
                break;
            case 3:
                if (BorderP4)
                    BorderP4->SetVisibility(ESlateVisibility::Visible);
                break;
            default:
                break;
            }
        }
    }
}



void UHUD_CharacterSelectTile::StartShake()
{
    ShakeStep = 0;
    GetWorld()->GetTimerManager().SetTimer(ShakeTimerHandle, this, &UHUD_CharacterSelectTile::TickShake, 0.02f, true);
}

void UHUD_CharacterSelectTile::TickShake()
{
    static const TArray<float> Offsets = { 0.f, -5.f, 5.f, -5.f, 0.f };

    if (ShakeStep < Offsets.Num())
    {
        FVector2D NewOffset(Offsets[ShakeStep], 0.f);
        SetRenderTranslation(NewOffset);
        ShakeStep++;
    }
    else
    {
        GetWorld()->GetTimerManager().ClearTimer(ShakeTimerHandle);
        SetRenderTranslation(FVector2D::ZeroVector);
    }
}
