// GI_GameCoreInstance.cpp
#include "GI_GameCoreInstance.h"

UGI_GameCoreInstance::UGI_GameCoreInstance()
{
	SelectedPlayMode = EPlayMode::None;
	SelectedCharacterID = TEXT("");
	SelectedMapName = TEXT("");
	LocalPlayerName = TEXT("Player");
	bIsHost = false;
}
