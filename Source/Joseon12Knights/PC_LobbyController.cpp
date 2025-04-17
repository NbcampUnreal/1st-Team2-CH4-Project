#include "PC_LobbyController.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerState.h"
#include "PS_FighterPlayerState.h"
#include "GM_LobbyMode.h"
#include "Blueprint/UserWidget.h"
#include "GS_FighterState.h"
#include "Camera/CameraActor.h"
#include "EngineUtils.h"

void APC_LobbyController::BeginPlay()
{
    Super::BeginPlay();

    if (IsLocalController()) 
    {
        if (LobbyHUDClass)
        {
            LobbyHUD = CreateWidget<UHUD_ReadyText>(this, LobbyHUDClass);
            if (LobbyHUD)
            {
                LobbyHUD->AddToViewport();
                UE_LOG(LogTemp, Warning, TEXT("✅ LobbyHUD 표시됨"));
            }
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("❌ LobbyHUDClass가 설정되지 않음"));
        }
    }

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
    UE_LOG(LogTemp, Warning, TEXT("▶ Enter 키 눌림"));

    if (IsLocalController())
    {
        UE_LOG(LogTemp, Warning, TEXT("▶ 로컬 컨트롤러 확인됨"));

        if (LobbyHUD)
        {
            if (APS_FighterPlayerState* PS = GetPlayerState<APS_FighterPlayerState>())
            {
                UE_LOG(LogTemp, Warning, TEXT("▶ PlayerIndex: %d"), PS->LobbyPlayerIndex);
                LobbyHUD->ShowReadyForPlayer(PS->LobbyPlayerIndex);
            }
            else
            {
                UE_LOG(LogTemp, Error, TEXT("❌ FighterPlayerState 없음"));
            }
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("❌ LobbyHUD null 상태"));
        }
    }

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

        int32 PlayerIndex = PS->LobbyPlayerIndex;

        for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
        {
            if (APC_LobbyController* OtherPC = Cast<APC_LobbyController>(*It))
            {
                OtherPC->Client_ShowReadyUI(PlayerIndex);
            }
        }
    }
}


void APC_LobbyController::Server_AttemptStartMatch_Implementation()
{
    if (AGM_LobbyMode* GM = Cast<AGM_LobbyMode>(UGameplayStatics::GetGameMode(this)))
    {
        GM->TryStartMatch();  
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

void APC_LobbyController::Client_SetSelectedCharacterID_Implementation(const FString& InCharacterID)
{
    UGI_GameCoreInstance* GI = Cast<UGI_GameCoreInstance>(UGameplayStatics::GetGameInstance(this));
    if (GI)
    {
        GI->SelectedCharacterID = InCharacterID;
    }
}


void APC_LobbyController::Client_ShowReadyUI_Implementation(int32 PlayerIndex)
{
    if (LobbyHUD)
    {
        LobbyHUD->ShowReadyForPlayer(PlayerIndex);
    }
}
