// GM_StoryMode.cpp
#include "GM_StoryMode.h"
#include "Kismet/GameplayStatics.h"
#include "GI_GameCoreInstance.h"

void AGM_StoryMode::BeginPlay()
{
    Super::BeginPlay();

    StartCharacterSelect();
}

void AGM_StoryMode::ProceedToMatch()
{
    if (UGI_GameCoreInstance* GI = GetGameInstance<UGI_GameCoreInstance>())
    {
        if (GI->SelectedMap.IsValid())
        {
            FString AssetName = GI->SelectedMap.GetAssetName();
            UGameplayStatics::OpenLevel(this, FName(*AssetName));
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("StoryMode ProceedToMatch: 선택된 맵이 유효하지 않음"));
        }
    }
}
