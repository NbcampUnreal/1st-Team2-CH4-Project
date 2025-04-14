#include "HUD_OnlineMenus.h"
#include "Components/Button.h"
#include "PC_MenuController.h"
#include "PC_LobbyController.h"

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
    if (LobbyMap.IsValid() || LobbyMap.ToSoftObjectPath().IsValid())
    {
        // 강제 로딩
        LobbyMap.LoadSynchronous();

        FString MapName = LobbyMap.GetAssetName();

        if (APlayerController* PC = GetOwningPlayer())
        {
            FString Command = FString::Printf(TEXT("open %s?listen"), *MapName);
            UE_LOG(LogTemp, Warning, TEXT("🔥 Final Command: %s"), *Command);
            PC->ConsoleCommand(Command);
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("❌ LobbyMap 설정이 유효하지 않음 또는 경로 불완전"));
    }

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


