#include "GM_LobbyMode.h"
#include "GI_GameCoreInstance.h"
#include "GS_FighterState.h"
#include "PS_FighterPlayerState.h"
#include "BP_LobbyCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

void AGM_LobbyMode::BeginPlay()
{
    Super::BeginPlay();
    GameInstance = Cast<UGI_GameCoreInstance>(GetGameInstance());
    SpawnedCount = 0;
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
        UE_LOG(LogTemp, Warning, TEXT("✅ All players ready. Starting match..."));
        StartMatch();
    }
}

void AGM_LobbyMode::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);

    if (!NewPlayer || !NewPlayer->PlayerState) return;

    FString CharacterID = "1";
    if (APS_FighterPlayerState* PS = Cast<APS_FighterPlayerState>(NewPlayer->PlayerState))
    {
        if (!PS->SelectedCharacterID.IsEmpty())
        {
            CharacterID = PS->SelectedCharacterID;
        }
    }

    if (!CharacterBPMap.Contains(CharacterID))
    {
        UE_LOG(LogTemp, Error, TEXT("❌ No blueprint found for CharacterID: %s"), *CharacterID);
        return;
    }

    TSubclassOf<AActor> BPClass = CharacterBPMap[CharacterID];
    FVector SpawnLocation = FVector(0.f, SpawnedCount * 200.f, 100.f);
    FRotator SpawnRotation = FRotator::ZeroRotator;

    AActor* SpawnedChar = GetWorld()->SpawnActor<AActor>(BPClass, SpawnLocation, SpawnRotation);
    if (ABP_LobbyCharacter* LobbyChar = Cast<ABP_LobbyCharacter>(SpawnedChar))
    {
        LobbyChar->SetPlayerIndex(SpawnedCount);
        LobbyChar->SetIsLocal(NewPlayer->IsLocalController());
        SpawnedLobbyCharacters.Add(LobbyChar);
    }

    SpawnedCount++;
}

void AGM_LobbyMode::StartMatch()
{
    if (AGS_FighterState* GS = GetGameState<AGS_FighterState>())
    {
        GS->bShowCharacterSelect = true;
        GS->MatchPhase = EMatchPhase::CharacterSelect;
    }

    UE_LOG(LogTemp, Log, TEXT("🚀 캐릭터 셀렉트 상태로 진입"));
}

void AGM_LobbyMode::Server_UpdatePlayers_Implementation(int32 NumPlayers)
{
    UE_LOG(LogTemp, Log, TEXT("📡 Server_UpdatePlayers_Implementation: %d players"), NumPlayers);
}

void AGM_LobbyMode::Client_UpdatePlayers_Implementation(int32 NumPlayers)
{
    UE_LOG(LogTemp, Log, TEXT("📡 Client_UpdatePlayers_Implementation: %d players"), NumPlayers);
}

void AGM_LobbyMode::Server_SetReady_Implementation(APlayerController* PC)
{
    if (PC && PC->PlayerState)
    {
        if (APS_FighterPlayerState* FighterPS = Cast<APS_FighterPlayerState>(PC->PlayerState))
        {
            FighterPS->bIsReady = true;
            UE_LOG(LogTemp, Log, TEXT("✅ %s is now ready"), *FighterPS->GetPlayerName());
        }
    }
}
