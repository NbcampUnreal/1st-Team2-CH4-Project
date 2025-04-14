#include "GM_LobbyMode.h"
#include "GI_GameCoreInstance.h"
#include "Kismet/GameplayStatics.h"
#include "GS_FighterState.h"
#include "PS_FighterPlayerState.h"
#include "EngineUtils.h"
#include "TimerManager.h"

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
        UE_LOG(LogTemp, Warning, TEXT("All players ready. Starting match..."));
        StartMatch();
    }
}

void AGM_LobbyMode::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);

    FTimerHandle TempHandle;
    GetWorldTimerManager().SetTimer(TempHandle, [this]()
    {
        int32 NumPlayers = GetNumPlayers();
        UE_LOG(LogTemp, Log, TEXT("Player joined. Total Players: %d"), NumPlayers);
        // 필요하면 GameState를 통해 클라에 동기화 가능
    }, 0.1f, false);
}

void AGM_LobbyMode::StartMatch()
{
    if (AGS_FighterState* GS = GetGameState<AGS_FighterState>())
    {
        GS->bShowCharacterSelect = true;
        GS->MatchPhase = EMatchPhase::CharacterSelect;
    }

    UE_LOG(LogTemp, Log, TEXT("캐릭터 셀렉트 상태로 진입"));
}

void AGM_LobbyMode::Server_UpdatePlayers_Implementation(int32 NumPlayers)
{
    // Manager 제거되었으므로 비워두거나 GameState로 구현할 것
    UE_LOG(LogTemp, Log, TEXT("Server_UpdatePlayers_Implementation called. NumPlayers: %d"), NumPlayers);
}

void AGM_LobbyMode::Client_UpdatePlayers_Implementation(int32 NumPlayers)
{
    // Manager 제거되었으므로 비워두거나 UI 갱신용 GameState 활용 가능
    UE_LOG(LogTemp, Log, TEXT("Client_UpdatePlayers_Implementation called. NumPlayers: %d"), NumPlayers);
}

void AGM_LobbyMode::Server_SetReady_Implementation(APlayerController* PC)
{
    if (PC && PC->PlayerState)
    {
        if (APS_FighterPlayerState* FighterPS = Cast<APS_FighterPlayerState>(PC->PlayerState))
        {
            FighterPS->bIsReady = true;
            UE_LOG(LogTemp, Log, TEXT("%s is now ready"), *FighterPS->GetPlayerName());
        }
    }
}
