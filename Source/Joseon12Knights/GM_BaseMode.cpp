// GM_BaseMode.cpp
#include "GM_BaseMode.h"

void AGM_BaseMode::StartCharacterSelect()
{
	UE_LOG(LogTemp, Log, TEXT("StartCharacterSelect called (BaseMode)"));
}

void AGM_BaseMode::OnCharacterSelected(APlayerController* PC, const FString& CharacterID)
{
	if (UGI_GameCoreInstance* GI = GetGI())
	{
		GI->SelectedCharacterID = CharacterID;
	}
}

void AGM_BaseMode::StartMapSelect()
{
	UE_LOG(LogTemp, Log, TEXT("StartMapSelect called (BaseMode)"));
}

void AGM_BaseMode::OnMapSelected(const FString& MapName)
{
	if (UGI_GameCoreInstance* GI = GetGI())
	{
		GI->SelectedMapName = MapName;
	}
}

void AGM_BaseMode::ProceedToMatch()
{
	UE_LOG(LogTemp, Log, TEXT("ProceedToMatch called (BaseMode)"));
}

UGI_GameCoreInstance* AGM_BaseMode::GetGI() const
{
	return Cast<UGI_GameCoreInstance>(GetGameInstance());
}


AGS_FighterState* AGM_BaseMode::GetGS() const
{
	return GetGameState<AGS_FighterState>();
}

