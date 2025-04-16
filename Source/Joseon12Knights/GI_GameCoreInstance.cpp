#include "GI_GameCoreInstance.h"
#include "Kismet/GameplayStatics.h"

UGI_GameCoreInstance::UGI_GameCoreInstance()
{
	SelectedPlayMode = EPlayMode::None;
	SelectedCharacterID = TEXT("");
	SelectedMapName = TEXT("");
	LocalPlayerName = TEXT("Player");
	bIsHost = false;
}



