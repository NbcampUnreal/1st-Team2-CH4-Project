#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSubsystem.h"
#include "GI_GameCoreInstance.generated.h"

UENUM(BlueprintType)
enum class EPlayMode : uint8
{
	None,
	Story,
	Single,
	Online
};

USTRUCT(BlueprintType)
struct FPlayerLobbyInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FString PlayerName;

	UPROPERTY(BlueprintReadWrite)
	bool bIsReady;

	UPROPERTY(BlueprintReadWrite)
	FString SelectedCharacterID;
};

UCLASS()
class JOSEON12KNIGHTS_API UGI_GameCoreInstance : public UGameInstance
{
	GENERATED_BODY()

private:
	IOnlineSessionPtr SessionInterface;
	virtual void Init() override;

public:
	UGI_GameCoreInstance();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSoftObjectPtr<UWorld> SelectedMap;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EPlayMode SelectedPlayMode;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString SelectedCharacterID;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 SelectedCpuCount = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FString> CpuCharacterIDs;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString SelectedMapName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString LocalPlayerName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FPlayerLobbyInfo> PlayerLobbyInfos;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bIsHost;

	UPROPERTY(BlueprintReadWrite)
	TArray<bool> ReadyArray;

	UPROPERTY(BlueprintReadWrite)
	FName CustomSessionName;

	void CreateSession();
	void JoinSession(const FOnlineSessionSearchResult& SessionResult);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	void CreateSessionWithName(FName InName);
	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);
};
