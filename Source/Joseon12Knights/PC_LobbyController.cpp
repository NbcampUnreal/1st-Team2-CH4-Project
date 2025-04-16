#include "PC_LobbyController.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerState.h"
#include "PS_FighterPlayerState.h"
#include "GM_LobbyMode.h"
#include "GS_FighterState.h"
#include "Camera/CameraActor.h"
#include "EngineUtils.h"

void APC_LobbyController::BeginPlay()
{
    Super::BeginPlay();
    SetLobbyCameraView();

    FInputModeGameAndUI InputMode;
    InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
    InputMode.SetHideCursorDuringCapture(false);
    SetInputMode(InputMode);

    bShowMouseCursor = true;
    PrimaryActorTick.bCanEverTick = true;
}

void APC_LobbyController::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
}

void APC_LobbyController::SetupInputComponent()
{
    Super::SetupInputComponent();

    if (InputComponent)
    {
        InputComponent->BindAction("LobbyEnter", IE_Pressed, this, &APC_LobbyController::HandleEnterKey);
    }

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
    }
}

void APC_LobbyController::Server_AttemptStartMatch_Implementation()
{
    if (AGM_LobbyMode* GM = Cast<AGM_LobbyMode>(UGameplayStatics::GetGameMode(this)))
    {
        AGS_FighterState* GS = Cast<AGS_FighterState>(UGameplayStatics::GetGameState(this));
        if (!GS)
        {
            return;
        }

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
            // 바로 인게임 맵으로 전환
            UGameplayStatics::OpenLevel(this, FName("Concluding_Ground_Online1")); // 실제 맵 이름으로 교체 필요
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
