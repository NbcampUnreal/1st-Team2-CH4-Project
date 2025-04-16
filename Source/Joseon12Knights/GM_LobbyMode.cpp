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
        TryStartMatch();
    }
}

AActor* AGM_LobbyMode::ChoosePlayerStart_Implementation(AController* Player)
{
    int32 PlayerIndex = PlayerNumberMap.Contains(Player) ? PlayerNumberMap[Player] : GetNumPlayers() - 1;

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

    // 플레이어 번호 계산
    int32 PlayerIndex = GetNumPlayers() - 1; 
    PlayerNumberMap.Add(NewPlayer, PlayerIndex);

    FString CharacterKey = FString::FromInt(PlayerIndex + 1);

    if (!CharacterBPMap.Contains(CharacterKey))
    {
        UE_LOG(LogTemp, Error, TEXT("❌ No blueprint found for CharacterKey: %s"), *CharacterKey);
        return;
    }

    TSubclassOf<AActor> BPClass = CharacterBPMap[CharacterKey];

    // PlayerStart 찾기
    FString StartName = PlayerIndex == 0 ? TEXT("PlayerStart") : FString::Printf(TEXT("PlayerStart%d"), PlayerIndex + 1);
    AActor* PlayerStart = nullptr;

    for (TActorIterator<APlayerStart> It(GetWorld()); It; ++It)
    {
        if (It->GetName().Equals(StartName, ESearchCase::IgnoreCase))
        {
            PlayerStart = *It;
            break;
        }
    }

    if (!PlayerStart)
    {
        UE_LOG(LogTemp, Error, TEXT("❌ No PlayerStart found with name %s"), *StartName);
        return;
    }

    FVector SpawnLocation = PlayerStart->GetActorLocation();
    FRotator SpawnRotation = PlayerStart->GetActorRotation();

    // 캐릭터 스폰
    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = NewPlayer;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    AActor* SpawnedChar = GetWorld()->SpawnActor<AActor>(BPClass, SpawnLocation, SpawnRotation, SpawnParams);
    if (ABP_LobbyCharacter* LobbyChar = Cast<ABP_LobbyCharacter>(SpawnedChar))
    {
        LobbyChar->SetPlayerIndex(PlayerIndex);
        LobbyChar->SetIsLocal(NewPlayer->IsLocalController());
        SpawnedLobbyCharacters.Add(LobbyChar);

    }


    if (UGI_GameCoreInstance* GI = Cast<UGI_GameCoreInstance>(GetGameInstance()))
    {
        GI->PlayerCharacterMap.Add(PlayerIndex, CharacterKey);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("GameInstance 캐스팅 실패"));
    }
}



void AGM_LobbyMode::TryStartMatch()
{

    FString TravelURL = TEXT("/Game/PlatformFighterKit/Maps/Levels/Concluding_Ground_Online1?listen");

    if (HasAuthority())
    {
        GetWorld()->ServerTravel(TravelURL, true);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("TryStartMatch: 클라이언트에서 호출됨"));
    }
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