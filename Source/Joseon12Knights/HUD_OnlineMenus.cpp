#include "HUD_OnlineMenus.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/VerticalBox.h"
#include "GI_GameCoreInstance.h"
#include "Kismet/GameplayStatics.h"

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

	if (JoinButton)
	{
		JoinButton->OnClicked.AddDynamic(this, &UHUD_OnlineMenus::OnClickJoin);
	}

	if (CreateSessionButton)
	{
		CreateSessionButton->OnClicked.AddDynamic(this, &UHUD_OnlineMenus::OnCreateSessionConfirmed);
	}

	if (SessionName && SessionNamePanel)
	{
		SessionNamePanel->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UHUD_OnlineMenus::OnHostClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("▶ Host 버튼 클릭됨"));

	if (SessionNamePanel)
	{
		SessionNamePanel->SetVisibility(ESlateVisibility::Visible);
		UE_LOG(LogTemp, Warning, TEXT("✅ SessionNamePanel SetVisibility → Visible"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("❌ SessionNamePanel is nullptr"));
	}
}

void UHUD_OnlineMenus::OnCreateSessionConfirmed()
{
	if (!SessionName || SessionName->GetText().IsEmpty()) return;

	FString EnteredName = SessionName->GetText().ToString();

	if (UGI_GameCoreInstance* GI = GetGameInstance<UGI_GameCoreInstance>())
	{
		GI->bIsHost = true;
		UE_LOG(LogTemp, Warning, TEXT("🟢 Host 설정됨: bIsHost = true"));
		GI->SelectedPlayMode = EPlayMode::Online;
		GI->CustomSessionName = FName(*EnteredName);
		GI->CreateSessionWithName(GI->CustomSessionName);
	}
}

void UHUD_OnlineMenus::OnClickJoin()
{
	UE_LOG(LogTemp, Warning, TEXT("Join 버튼 클릭됨"));
}

void UHUD_OnlineMenus::OnBackClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("뒤로가기 버튼 클릭됨"));

	if (APlayerController* PC = GetOwningPlayer())
	{
		if (APC_MenuController* MenuPC = Cast<APC_MenuController>(PC))
		{
			MenuPC->HandleBackToMainMenu();  // 🔁 메인 메뉴로 복귀
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("❌ MenuController 캐스팅 실패"));
		}
	}
}
