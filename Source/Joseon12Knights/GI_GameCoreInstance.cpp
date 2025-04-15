#include "GI_GameCoreInstance.h"
#include "Kismet/GameplayStatics.h"

UGI_GameCoreInstance::UGI_GameCoreInstance()
{
	SelectedPlayMode = EPlayMode::None;
	SelectedCharacterID = TEXT("");
	SelectedMapName = TEXT("");
	LocalPlayerName = TEXT("Player");
	bIsHost = false;
}

void UGI_GameCoreInstance::Init()
{
	Super::Init();

	if (IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get())
	{
		SessionInterface = Subsystem->GetSessionInterface();

		if (SessionInterface.IsValid())
		{
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UGI_GameCoreInstance::OnCreateSessionComplete);
			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UGI_GameCoreInstance::OnJoinSessionComplete);
		}
	}
}

void UGI_GameCoreInstance::CreateSession()
{
	if (!SessionInterface.IsValid()) return;

	FOnlineSessionSettings SessionSettings;
	SessionSettings.bIsLANMatch = true;
	SessionSettings.NumPublicConnections = 4;
	SessionSettings.bShouldAdvertise = true;
	SessionSettings.bUsesPresence = false;

	SessionInterface->CreateSession(0, NAME_GameSession, SessionSettings);
}

void UGI_GameCoreInstance::CreateSessionWithName(FName InName)
{
	UE_LOG(LogTemp, Warning, TEXT("📦 CreateSessionWithName 진입, bIsHost = %s"), bIsHost ? TEXT("true") : TEXT("false"));

	if (!bIsHost)
	{
		UE_LOG(LogTemp, Error, TEXT("❌ bIsHost == false 인데 CreateSession 시도됨!"));
		return;
	}

	if (!SessionInterface.IsValid()) return;

	// 기존 세션 존재 시 Destroy 후 재시도
	if (SessionInterface->GetNamedSession(InName))
	{
		UE_LOG(LogTemp, Warning, TEXT("⚠️ 세션 '%s' 이 이미 존재함 → Destroy 후 재시도 예약"), *InName.ToString());

		SessionInterface->DestroySession(InName);

		FTimerHandle RetryHandle;
		GetWorld()->GetTimerManager().SetTimer(RetryHandle, [this, InName]()
		{
			UE_LOG(LogTemp, Warning, TEXT("♻️ Destroy 이후 CreateSession 재시도: %s"), *InName.ToString());
			CreateSessionWithName(InName);
		}, 0.5f, false);

		return;
	}

	// 세션 설정 및 생성
	FOnlineSessionSettings SessionSettings;
	SessionSettings.bIsLANMatch = true;
	SessionSettings.NumPublicConnections = 4;
	SessionSettings.bShouldAdvertise = true;
	SessionSettings.bUsesPresence = false;
	SessionSettings.Set(FName("SESSION_NAME"), InName.ToString(), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

	SessionInterface->CreateSession(0, InName, SessionSettings);
}



void UGI_GameCoreInstance::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	UE_LOG(LogTemp, Warning, TEXT("✅ 세션 생성 성공: %s"), *SessionName.ToString());

	if (bWasSuccessful && bIsHost)
	{
		UE_LOG(LogTemp, Warning, TEXT("🌍 Host: OpenLevel 실행"));

		UGameplayStatics::OpenLevel(GetWorld(), FName("OnlineLobby"), true, "listen");
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("❌ 세션 생성 실패 또는 bIsHost 아님"));
	}
}



void UGI_GameCoreInstance::JoinSession(const FOnlineSessionSearchResult& SessionResult)
{
	if (!SessionInterface.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("❌ 세션 인터페이스 없음"));
		return;
	}

	if (!SessionResult.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("❌ 세션 결과가 유효하지 않음 (SessionResult invalid)"));
		return;
	}

	if (SessionInterface->GetNamedSession(NAME_GameSession))
	{
		UE_LOG(LogTemp, Warning, TEXT("⚠️ 이미 GameSession 존재 → Join 생략"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("📡 JoinSession 요청"));
	SessionInterface->JoinSession(0, NAME_GameSession, SessionResult);
}



void UGI_GameCoreInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	UE_LOG(LogTemp, Warning, TEXT("📡 OnJoinSessionComplete: %s, Result: %d"), *SessionName.ToString(), (int32)Result);

	if (Result != EOnJoinSessionCompleteResult::Success)
	{
		UE_LOG(LogTemp, Error, TEXT("❌ JoinSession 실패. 사유 코드: %d (0=Success, 1=AlreadyInSession, 2=SessionIsFull, 3=NotFound, 4=SessionFailure, 5=UnknownError)"), (int32)Result);
		//return;
	}

	FString ConnectInfo;
	if (SessionInterface->GetResolvedConnectString(SessionName, ConnectInfo))
	{
		UE_LOG(LogTemp, Warning, TEXT("🚀 접속 주소 확인 성공: %s"), *ConnectInfo);
		if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
		{
			UE_LOG(LogTemp, Warning, TEXT("🎮 ClientTravel 시도"));
			PC->ClientTravel(ConnectInfo, ETravelType::TRAVEL_Absolute);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("❌ PlayerController 찾기 실패"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("❌ 접속 주소 얻기 실패 (SessionName: %s) → 재시도 필요 가능성 있음"), *SessionName.ToString());
	}

}

