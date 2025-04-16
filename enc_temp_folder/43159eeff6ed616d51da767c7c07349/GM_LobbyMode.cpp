#include "GM_LobbyMode.h"
#include "GI_GameCoreInstance.h"
#include "GS_FighterState.h"
#include "PS_FighterPlayerState.h"
#include "GameFramework/PlayerStart.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"

void AGM_LobbyMode::BeginPlay()
{
    Super::BeginPlay();
    GameInstance = Cast<UGI_GameCoreInstance>(GetGameInstance());

}

void AGM_LobbyMode::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    if (bStartClosed) return;

    AGS_FighterState* GS = GetGameState<AGS_FighterState>();
    if (!GS) return;

    bool bAllReady = true;
    for (APlayerState* PS : GS->PlayerArray)
    {
        if (APS_FighterPlayerState* FighterPS = Cast<APS_FighterPlayerState>(PS))
        {
            if (!FighterPS->bIsReady)
            {
                bAllReady = false;
                break;
            }
        }
    }

    if (bAllReady && GS->PlayerArray.Num() > 0)
    {
        bStartClosed = true;
        StartMatch();
    }
}

AActor* AGM_LobbyMode::ChoosePlayerStart_Implementation(AController* Player)
{
    int32 PlayerIndex = GetNumPlayers() - 1;
    PlayerNumberMap.Add(Player, PlayerIndex);

    FString StartName = PlayerIndex == 0 ? TEXT("PlayerStart") : FString::Printf(TEXT("PlayerStart%d"), PlayerIndex + 1);

    for (TActorIterator<APlayerStart> It(GetWorld()); It; ++It)
    {
        if (It->GetName().Equals(StartName, ESearchCase::IgnoreCase))
        {
            return *It;
        }
    }

    UE_LOG(LogTemp, Error, TEXT("❌ PlayerStart named %s not found!"), *StartName);
    return Super::ChoosePlayerStart_Implementation(Player);
}

void AGM_LobbyMode::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);
    PlayerNumberMap.Add(NewPlayer, GetNumPlayers() - 1);
}

void AGM_LobbyMode::StartMatch()
{
    UGameplayStatics::OpenLevel(this, FName("Level_Online")); // 맵 이름은 실제 사용하는 맵으로 교체
}

void AGM_LobbyMode::Server_SetReady_Implementation(APlayerController* PC)
{
    if (PC && PC->PlayerState)
    {
        if (APS_FighterPlayerState* FighterPS = Cast<APS_FighterPlayerState>(PC->PlayerState))
        {
            FighterPS->bIsReady = true;
        }
    }
}