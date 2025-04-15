#include "WBP_ServerButton.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "GI_GameCoreInstance.h"

void UWBP_ServerButton::NativeConstruct()
{
	Super::NativeConstruct();

	if (JoinButton)
	{
		JoinButton->OnClicked.AddDynamic(this, &UWBP_ServerButton::OnJoinClicked);
	}
}

void UWBP_ServerButton::Setup(const FOnlineSessionSearchResult& InResult)
{
    Result = InResult;
    UE_LOG(LogTemp, Warning, TEXT("✅ Setup 호출됨 - IsValid = %s"), Result.IsValid() ? TEXT("true") : TEXT("false"));
    FString SessionName;
    if (Result.Session.SessionSettings.Get(FName("SESSION_NAME"), SessionName))
    {
        if (SessionNameText)
        {
            SessionNameText->SetText(FText::FromString(SessionName));
        }
        UE_LOG(LogTemp, Log, TEXT("✅ 세션 이름 설정: %s"), *SessionName);
    }
    else
    {
        if (SessionNameText)
        {
            SessionNameText->SetText(FText::FromString(TEXT("이름 없음")));
        }
        UE_LOG(LogTemp, Warning, TEXT("⚠️ 세션 이름을 가져오지 못함"));
    }
}


void UWBP_ServerButton::OnJoinClicked()
{
    if (!Result.IsValid())
    {
        UE_LOG(LogTemp, Error, TEXT("❌ JoinButton 눌렀지만 SessionResult 무효"));
        return;
    }

    if (UGI_GameCoreInstance* GI = GetGameInstance<UGI_GameCoreInstance>())
    {
        GI->bIsHost = false;
        GI->JoinSession(Result);
        UE_LOG(LogTemp, Log, TEXT("✅ JoinSession 호출 성공"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("❌ GameInstance를 가져오지 못함"));
    }
}
