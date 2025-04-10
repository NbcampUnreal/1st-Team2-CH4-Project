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

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EPlayMode SelectedPlayMode;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString SelectedCharacterID;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString SelectedMapName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString LocalPlayerName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FPlayerLobbyInfo> PlayerLobbyInfos;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bIsHost;
};
