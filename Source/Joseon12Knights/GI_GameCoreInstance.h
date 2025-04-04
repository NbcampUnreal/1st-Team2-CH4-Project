// GI_GameCoreInstance.h
#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
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

public:
	UGI_GameCoreInstance();

	// 현재 선택된 모드
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EPlayMode SelectedPlayMode;

	// 로컬 플레이어의 캐릭터 ID
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString SelectedCharacterID;

	// 선택된 맵 이름
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString SelectedMapName;

	// 로컬 플레이어 이름
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString LocalPlayerName;

	// 온라인 모드용: 로비 플레이어 목록
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FPlayerLobbyInfo> PlayerLobbyInfos;

	// 온라인 모드용: 현재 Host 여부
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bIsHost;
};
