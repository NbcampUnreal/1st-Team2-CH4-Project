#include "HUD_CharacterSelect.h"
#include "Components/TextBlock.h"
#include "GS_FighterState.h"
#include "HUD_CharacterSelectPanel.h"
#include "HUD_CharacterSelectTile.h"
#include "Kismet/KismetMathLibrary.h"

void UHUD_CharacterSelect::NativeConstruct()
{
    Super::NativeConstruct();

    bPressStartPlayed = false; 

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

    if (UGI_GameCoreInstance* GI = Cast<UGI_GameCoreInstance>(GetGameInstance()))
    {
        FString CharID = GI->SelectedCharacterID;
        if (!CharID.IsEmpty())
        {
            NotifyCharacterSelected(CharID, 0);

            FString DisplayName = CharID;
            static TMap<FString, FString> NameMap = {
                {TEXT("1"), TEXT("자 기사")},
                {TEXT("2"), TEXT("축 기사")},
                {TEXT("3"), TEXT("인 기사")},
                {TEXT("4"), TEXT("진 기사")}
            };
            if (NameMap.Contains(CharID))
            {
                DisplayName = NameMap[CharID];
            }

            if (Pannel1)
            {
                Pannel1->SetCharacter(DisplayName, TEXT("1"));
            }
        }

        CpuCount = GI->CpuCharacterIDs.Num();
        UpdateCpuText();

        for (int32 i = 1; i <= CpuCount; ++i)
        {
            FString SavedID = GI->CpuCharacterIDs[i - 1];
            ApplyCpuCharacterToPanel(i, SavedID);
        }

        if (!bPressStartPlayed && !CharID.IsEmpty() && CpuCount >= 1 && PressStartAnim)
        {
            UE_LOG(LogTemp, Warning, TEXT("복구 시 조건 만족 → PressStart 애니메이션 재생"));
            PlayAnimation(PressStartAnim, 0.f, 1);
            bPressStartPlayed = true;
        }
    }
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

    int32 LocalPlayerIndex = 0;
    if (APlayerController* PC = GetOwningPlayer())
    {
        if (ULocalPlayer* LP = PC->GetLocalPlayer())
        {
            LocalPlayerIndex = LP->GetControllerId(); 
        }
    }

    if (!bPressStartPlayed && CpuCount >= 1 && PressStartAnim && PlayerIndex == LocalPlayerIndex)
    {
        PlayAnimation(PressStartAnim, 0.f, 1);
        bPressStartPlayed = true;
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

        if (UGI_GameCoreInstance* GI = Cast<UGI_GameCoreInstance>(GetGameInstance()))
        {
            GI->CpuCharacterIDs.Add(RandomID);
            GI->SelectedCpuCount = CpuCount;
        }

        int32 LocalPlayerIndex = 0;
        if (APlayerController* PC = GetOwningPlayer())
        {
            if (ULocalPlayer* LP = PC->GetLocalPlayer())
            {
                LocalPlayerIndex = LP->GetControllerId();
            }
        }

        if (!bPressStartPlayed && PressStartAnim)
        {
            for (const auto& Pair : CharacterSelections)
            {
                if (Pair.Value.Contains(LocalPlayerIndex))
                {
                    PlayAnimation(PressStartAnim, 0.f, 1);
                    bPressStartPlayed = true;
                    break;
                }
            }
        }
    }
}



void UHUD_CharacterSelect::OnCpuDown()
{
    if (CpuCount > 0)
    {
        ClearCpuCharacterFromPanel(CpuCount);
        CpuCount--;
        UpdateCpuText();

        if (UGI_GameCoreInstance* GI = Cast<UGI_GameCoreInstance>(GetGameInstance()))
        {
            if (GI->CpuCharacterIDs.IsValidIndex(CpuCount - 1))
            {
                GI->CpuCharacterIDs.RemoveAt(CpuCount - 1);
            }

            GI->SelectedCpuCount = CpuCount;
        }

        if (CpuCount == 0)
        {
            bPressStartPlayed = false;

            if (PressStartAnim)
            {
                StopAnimation(PressStartAnim);

                if (PressStart)
                {
                    PressStart->SetVisibility(ESlateVisibility::Hidden);
                }
            }
        }


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
        MaxCpuCount = 3;
    }
}

void UHUD_CharacterSelect::UpdateCpuText()
{
    if (CPU)
    {
        CPU->SetText(FText::AsNumber(CpuCount));
    }

    if (Hard)
    {
        switch (CpuCount)
        {
        case 1:
            Hard->SetText(FText::FromString(TEXT("고려")));
            break;
        case 2:
            Hard->SetText(FText::FromString(TEXT("금강")));
            break;
        case 3:
            Hard->SetText(FText::FromString(TEXT("태백")));
            break;
        default:
            Hard->SetText(FText::GetEmpty()); 
            break;
        }
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
    FString PlayerID = FString::Printf(TEXT("%d"), CpuIndex + 1);

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
    case 1: if (Pannel2) Pannel2->SetCharacter(TEXT("혼백"), TEXT("")); break;
    case 2: if (Pannel3) Pannel3->SetCharacter(TEXT("혼백"), TEXT("")); break;
    case 3: if (Pannel4) Pannel4->SetCharacter(TEXT("혼백"), TEXT("")); break;
    }
}

void UHUD_CharacterSelect::ApplyCpuCharacters()
{
    // 비어 있어도 OK
}
