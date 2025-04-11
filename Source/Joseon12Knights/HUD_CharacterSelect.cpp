#include "HUD_CharacterSelect.h"
#include "Components/TextBlock.h"
#include "GS_FighterState.h"
#include "HUD_CharacterSelectPanel.h"
#include "HUD_CharacterSelectTile.h"
#include "Kismet/KismetMathLibrary.h"

void UHUD_CharacterSelect::NativeConstruct()
{
    Super::NativeConstruct();

    Tiles = {
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
    if (CharacterPannel_7) CharacterPannel_7->SetupCharacterTile(TEXT("7"));
    if (CharacterPannel_8) CharacterPannel_8->SetupCharacterTile(TEXT("8"));

    if (BackButton)
        BackButton->OnClicked.AddDynamic(this, &UHUD_CharacterSelect::OnBackClicked);
}

void UHUD_CharacterSelect::OnBackClicked()
{
    RemoveFromParent();

    if (APlayerController* PC = GetOwningPlayer())
    {
        if (APC_MenuController* MenuPC = Cast<APC_MenuController>(PC))
        {
            MenuPC->HandleBackToMainMenu();
        }
    }
}

void UHUD_CharacterSelect::NotifyCharacterSelected(const FString& CharacterID, int32 PlayerIndex)
{
    for (auto& Pair : CharacterSelections)
    {
        Pair.Value.Remove(PlayerIndex);
    }

    CharacterSelections.FindOrAdd(CharacterID).Add(PlayerIndex);

    for (UHUD_CharacterSelectTile* Tile : Tiles)
    {
        if (Tile)
        {
            Tile->UpdateSelectionIndicators(CharacterSelections);
        }
    }
}


void UHUD_CharacterSelect::OnCpuUp()
{
    RecalculateMaxCpu();

    if (CpuCount < MaxCpuCount)
    {
        CpuCount++;
        UpdateCpuText();

        FString RandomID = GetAvailableRandomID();
        ApplyCpuCharacterToPanel(CpuCount, RandomID);
    }
}

void UHUD_CharacterSelect::OnCpuDown()
{
    if (CpuCount > 0)
    {
        ClearCpuCharacterFromPanel(CpuCount);
        CpuCount--;
        UpdateCpuText();
    }
}

void UHUD_CharacterSelect::RecalculateMaxCpu()
{
    if (const AGS_FighterState* GS = GetWorld()->GetGameState<AGS_FighterState>())
    {
        MaxCpuCount = FMath::Clamp(4 - GS->PlayerArray.Num(), 0, 3);
    }
    else
    {
        MaxCpuCount = 3; // 기본값 (싱글 모드 등)
    }
}

void UHUD_CharacterSelect::UpdateCpuText()
{
    if (CPU)
    {
        CPU->SetText(FText::AsNumber(CpuCount));
    }
}

FString UHUD_CharacterSelect::GetAvailableRandomID()
{
    TArray<FString> AllIDs = { "1", "2", "3", "4" };
    int32 Index = UKismetMathLibrary::RandomIntegerInRange(0, AllIDs.Num() - 1);
    return AllIDs[Index];
}

void UHUD_CharacterSelect::ApplyCpuCharacterToPanel(int32 CpuIndex, const FString& CharacterID)
{
    FString PlayerID = FString::Printf(TEXT("%d"), CpuIndex + 1);  // 예: 2 ~ 4

    static TMap<FString, FString> NameMap = {
        {TEXT("1"), TEXT("자 기사")},
        {TEXT("2"), TEXT("축 기사")},
        {TEXT("3"), TEXT("인 기사")},
        {TEXT("4"), TEXT("진 기사")}
    };
    FString DisplayName = NameMap.Contains(CharacterID) ? NameMap[CharacterID] : CharacterID;

    switch (CpuIndex)
    {
    case 1: if (Pannel2) Pannel2->SetCharacter(DisplayName, PlayerID); break;
    case 2: if (Pannel3) Pannel3->SetCharacter(DisplayName, PlayerID); break;
    case 3: if (Pannel4) Pannel4->SetCharacter(DisplayName, PlayerID); break;
    }
}


void UHUD_CharacterSelect::ClearCpuCharacterFromPanel(int32 CpuIndex)
{
    switch (CpuIndex)
    {
    case 1:
        if (Pannel2)
        {
            Pannel2->SetCharacter(TEXT("혼백"), TEXT("")); 
        }
        break;
    case 2:
        if (Pannel3)
        {
            Pannel3->SetCharacter(TEXT("혼백"), TEXT(""));
        }
        break;
    case 3:
        if (Pannel4)
        {
            Pannel4->SetCharacter(TEXT("혼백"), TEXT(""));
        }
        break;
    default:
        break;
    }
}




void UHUD_CharacterSelect::ApplyCpuCharacters()
{
    // 비어 있어도 OK. 반드시 정의만 되어 있으면 된다.
}
