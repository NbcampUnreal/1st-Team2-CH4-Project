#include "PC_LobbyController.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerState.h"
#include "PS_FighterPlayerState.h"
#include "GM_LobbyMode.h"
#include "GS_FighterState.h"
#include "Camera/CameraActor.h"
#include "Blueprint/UserWidget.h"
#include "EngineUtils.h"

void APC_LobbyController::BeginPlay()
{
    Super::BeginPlay();
    SetLobbyCameraView();

    bShowMouseCursor = true;
    SetInputMode(FInputModeUIOnly());
}

void APC_LobbyController::SetupInputComponent()
{
    Super::SetupInputComponent();
    InputComponent->BindAction("GameStart", IE_Pressed, this, &APC_LobbyController::HandleEnterKey);
}

void APC_LobbyController::HandleEnterKey()
{
    if (HasAuthority())
    {
        Server_AttemptStartMatch();
    }
    else
    {
        Server_PressReady();
    }
}

void APC_LobbyController::Server_PressReady_Implementation()
{
    if (APS_FighterPlayerState* PS = GetPlayerState<APS_FighterPlayerState>())
    {
        PS->bIsReady = true;
        UE_LOG(LogTemp, Log, TEXT("✅ %s is now ready"), *PS->GetPlayerName());
    }
}

void APC_LobbyController::Server_AttemptStartMatch_Implementation()
{
    if (AGM_LobbyMode* GM = Cast<AGM_LobbyMode>(UGameplayStatics::GetGameMode(this)))
    {
        AGS_FighterState* GS = Cast<AGS_FighterState>(UGameplayStatics::GetGameState(this));
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

        if (bAllReady)
        {
            GM->StartMatch();
        }
    }
}

void APC_LobbyController::SetLobbyCameraView()
{
    for (TActorIterator<ACameraActor> It(GetWorld()); It; ++It)
    {
        if (It->ActorHasTag(FName("LobbyCamera")))
        {
            SetViewTargetWithBlend(*It, 1.f);
            break;
        }
    }
}


