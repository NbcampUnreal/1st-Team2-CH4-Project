#include "GM_LobbyMode.h"
#include "GI_GameCoreInstance.h"
#include "GS_FighterState.h"
#include "PS_FighterPlayerState.h"
#include "BP_LobbyCharacter.h"
#include "GameFramework/PlayerStart.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"

void AGM_LobbyMode::BeginPlay()
{
    Super::BeginPlay();
    GameInstance = Cast<UGI_GameCoreInstance>(GetGameInstance());

    if (HasAuthority())
    {
        UE_LOG(LogTemp, Warning, TEXT("🏁 LobbyMode BeginPlay: HOST (HasAuthority = true)"));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("🎮 LobbyMode BeginPlay: CLIENT (HasAuthority = false)"));
    }
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

    int32 PlayerIndex = PlayerNumberMap.Contains(NewPlayer) ? PlayerNumberMap[NewPlayer] : 0;
    FString CharacterKey = FString::FromInt(PlayerIndex + 1); // "1", "2", "3", "4"

    if (!CharacterBPMap.Contains(CharacterKey))
    {
        UE_LOG(LogTemp, Error, TEXT("❌ No blueprint found for CharacterKey: %s"), *CharacterKey);
        return;
    }

    TSubclassOf<AActor> BPClass = CharacterBPMap[CharacterKey];

    AActor* PlayerStart = ChoosePlayerStart(NewPlayer);
    if (!PlayerStart)
    {
        UE_LOG(LogTemp, Error, TEXT("❌ No PlayerStart found for PlayerIndex %d"), PlayerIndex);
        return;
    }

    FVector SpawnLocation = PlayerStart->GetActorLocation();
    FRotator SpawnRotation = PlayerStart->GetActorRotation();

    AActor* SpawnedChar = GetWorld()->SpawnActor<AActor>(BPClass, SpawnLocation, SpawnRotation);
    if (ABP_LobbyCharacter* LobbyChar = Cast<ABP_LobbyCharacter>(SpawnedChar))
    {
        LobbyChar->SetPlayerIndex(PlayerIndex);
        LobbyChar->SetIsLocal(NewPlayer->IsLocalController());
        SpawnedLobbyCharacters.Add(LobbyChar);
    }
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


