#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "GS_FighterState.generated.h"

UENUM(BlueprintType)
enum class EMatchPhase : uint8
{
	Lobby,
	CharacterSelect,
	MapSelect,
	InMatch,
	Result
};

USTRUCT(BlueprintType)
struct FPlayerMatchInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	bool bIsAlive = true;

	UPROPERTY(BlueprintReadWrite)
	int32 Score = 0;
};

USTRUCT(BlueprintType)
struct FReplicatedPlayerMatchInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	APlayerState* PlayerState;

	UPROPERTY(BlueprintReadWrite)
	FPlayerMatchInfo Info;
};

UCLASS()
class JOSEON12KNIGHTS_API AGS_FighterState : public AGameStateBase
{
	GENERATED_BODY()

public:
	AGS_FighterState();

	// ���� ���� ���� �ܰ� (�κ�, ����, ��, �ΰ���, ���)
	UPROPERTY(ReplicatedUsing = OnRep_MatchPhase, BlueprintReadOnly)
	EMatchPhase MatchPhase;

	// ĳ���� ���� UI ����� ����
	UPROPERTY(Replicated, BlueprintReadOnly)
	bool bShowCharacterSelect = false;

	// ���θ���� ��� ���� UI ǥ�� ����
	UPROPERTY(Replicated, BlueprintReadOnly)
	bool bShowModeSelectUI = false;

	// �÷��̾� ���� ���� / ���� ���� (Ŭ�� ������)
	UPROPERTY(Replicated)
	TArray<FReplicatedPlayerMatchInfo> PlayerStates;

protected:
	UFUNCTION()
	void OnRep_MatchPhase();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

};

