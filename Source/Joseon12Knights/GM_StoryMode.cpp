#include "GM_StoryMode.h"
#include "Kismet/GameplayStatics.h"
#include "GI_GameCoreInstance.h"
#include "GameFramework/PlayerState.h"

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

    }
}

void AGM_StoryMode::HandlePlayerRespawn(AActor* PlayerActor)
{
    APawn* Pawn = Cast<APawn>(PlayerActor);
    if (!Pawn) return;

    AController* PC = Pawn->GetController();
    if (!PC) return;

    AMainPlayerState* PS = Cast<AMainPlayerState>(PC->PlayerState);
    if (!PS) return;

    int32 Lives = PS->GetStock();
    Lives--;
    PS->SetStock(Lives);

    if (Lives <= 0)
    {
        // 게임 종료 
    }
}

bool AGM_StoryMode::CanRespawn(AActor* PlayerActor) const
{
    APawn* Pawn = Cast<APawn>(PlayerActor);
    if (!Pawn) return false;

    AController* PC = Pawn->GetController();
    if (!PC) return false;

    AMainPlayerState* PS = Cast<AMainPlayerState>(PC->PlayerState);
    if (!PS) return false;

    return PS->GetStock() > 0;
}
