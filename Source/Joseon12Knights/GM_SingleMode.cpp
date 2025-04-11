// GM_SingleMode.cpp
#include "GM_SingleMode.h"
#include "Kismet/GameplayStatics.h"

void AGM_SingleMode::BeginPlay()
{
	Super::BeginPlay();

	StartCharacterSelect();
}

void AGM_SingleMode::ProceedToMatch()
{
	if (UGI_GameCoreInstance* GI = GetGameInstance<UGI_GameCoreInstance>())
	{
		if (GI->SelectedMap.IsValid())
		{
			FString AssetName = GI->SelectedMap.GetAssetName(); 

			UGameplayStatics::OpenLevel(this, FName(*AssetName));
		}

	}
}
