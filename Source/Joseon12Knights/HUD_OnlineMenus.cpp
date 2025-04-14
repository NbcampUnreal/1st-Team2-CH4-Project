#include "HUD_OnlineMenus.h"
#include "Components/Button.h"
#include "PC_MenuController.h"

void UHUD_OnlineMenus::NativeConstruct()
{
    Super::NativeConstruct();

    if (Back)
    {
        Back->OnClicked.AddDynamic(this, &UHUD_OnlineMenus::OnBackClicked);
    }

    if (HostButton)
    {
        HostButton->OnClicked.AddDynamic(this, &UHUD_OnlineMenus::OnHostClicked);
    }
}

void UHUD_OnlineMenus::OnHostClicked()
{
    if (UGI_GameCoreInstance* GI = GetGameInstance<UGI_GameCoreInstance>())
    {
        GI->bIsHost = true;
        GI->SelectedPlayMode = EPlayMode::Online;
    }

    // listen 서버로 로비 맵 오픈
    UE_LOG(LogTemp, Warning, TEXT("🚀 HostButton clicked — Opening LobbyMap in listen mode"));
    UGameplayStatics::OpenLevel(this, TEXT("OnlineLobby"), true, TEXT("listen"));
}

void UHUD_OnlineMenus::OnBackClicked()
{
    RemoveFromParent();

    if (APlayerController* PC = GetOwningPlayer())
    {
        if (APC_MenuController* MenuPC = Cast<APC_MenuController>(PC))
        {
            MenuPC->HandleBackToMainMenu();  // 메인 메뉴로 이동
        }
    }
}
