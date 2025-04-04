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

	// ���� ���õ� ���
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EPlayMode SelectedPlayMode;

	// ���� �÷��̾��� ĳ���� ID
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString SelectedCharacterID;

	// ���õ� �� �̸�
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString SelectedMapName;

	// ���� �÷��̾� �̸�
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString LocalPlayerName;

	// �¶��� ����: �κ� �÷��̾� ���
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FPlayerLobbyInfo> PlayerLobbyInfos;

	// �¶��� ����: ���� Host ����
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bIsHost;
};
