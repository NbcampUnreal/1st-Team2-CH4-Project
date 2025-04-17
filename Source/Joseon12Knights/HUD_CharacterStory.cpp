#include "HUD_CharacterStory.h"
#include "HUD_CharacterSelectTile.h"
#include "HUD_CharacterSelectPanel.h"
#include "Components/Button.h"
#include "PC_MenuController.h"

void UHUD_CharacterStory::NativeConstruct()
{
	Super::NativeConstruct();

	Tiles = {
		CharacterPannel_1, CharacterPannel_2, CharacterPannel_3, CharacterPannel_4,
		CharacterPannel_5, CharacterPannel_6, CharacterPannel_7, CharacterPannel_8
	};

	const TArray<FString> CharacterIDs = {
		TEXT("1"), TEXT("2"), TEXT("3"), TEXT("4"),
		TEXT("5"), TEXT("6"), TEXT("7"), TEXT("8")
	};

	for (int32 i = 0; i < Tiles.Num(); ++i)
	{
		if (Tiles[i])
		{
			Tiles[i]->SetupCharacterTile(CharacterIDs[i]);
			Tiles[i]->Panel1 = Panel1;
		}
	}

	// 뒤로 가기 버튼
	if (BackButton)
	{
		BackButton->OnClicked.AddDynamic(this, &UHUD_CharacterStory::HandleBack);
	}
}

void UHUD_CharacterStory::HandleBack()
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

void UHUD_CharacterStory::NotifyCharacterSelected(const FString& CharacterID, int32 PlayerIndex)
{
	SelectedCharacterID = CharacterID;

	TMap<FString, TArray<int32>> TempSelections;
	TempSelections.FindOrAdd(CharacterID).Add(PlayerIndex);

	for (UHUD_CharacterSelectTile* Tile : Tiles)
	{
		if (Tile)
		{
			Tile->UpdateSelectionIndicators(TempSelections);
		}
	}

	if (Panel1)
	{
		static TMap<FString, FString> NameMap = {
			{TEXT("1"), TEXT("자 기사")},
			{TEXT("2"), TEXT("축 기사")},
			{TEXT("3"), TEXT("인 기사")},
			{TEXT("4"), TEXT("진 기사")}
		};

		FString DisplayName = NameMap.Contains(CharacterID) ? NameMap[CharacterID] : CharacterID;
		Panel1->SetCharacter(DisplayName, TEXT("1"));
	}

	if (PressStartAnim)
	{
		PlayAnimation(PressStartAnim, 0.f, 1);
	}
}


void UHUD_CharacterStory::ConfirmSelection()
{
	if (SelectedCharacterID.IsEmpty())
	{
		return;
	}

	if (UGI_GameCoreInstance* GI = Cast<UGI_GameCoreInstance>(GetGameInstance()))
	{
		GI->SelectedCharacterID = SelectedCharacterID;
		GI->SelectedPlayMode = EPlayMode::Story;
		GI->SelectedMap = StoryMapAsset;

		GI->PlayerLobbyInfos.Empty();
		FPlayerLobbyInfo Info;
		Info.SelectedCharacterID = SelectedCharacterID;
		GI->PlayerLobbyInfos.Add(Info);

		const FString MapName = StoryMapAsset.ToSoftObjectPath().GetAssetName();
		UGameplayStatics::OpenLevel(this, FName(*MapName));
	}
}
