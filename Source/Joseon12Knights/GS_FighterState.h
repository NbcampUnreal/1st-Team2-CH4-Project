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

	// 현재 게임 상태 단계 (로비, 셀렉, 맵, 인게임, 결과)
	UPROPERTY(ReplicatedUsing = OnRep_MatchPhase, BlueprintReadOnly)
	EMatchPhase MatchPhase;

	// 캐릭터 셀렉 UI 띄울지 여부
	UPROPERTY(Replicated, BlueprintReadOnly)
	bool bShowCharacterSelect = false;

	// 메인모드의 모드 선택 UI 표시 여부
	UPROPERTY(Replicated, BlueprintReadOnly)
	bool bShowModeSelectUI = false;

	// 플레이어 개별 점수 / 생존 상태 (클라 복제용)
	UPROPERTY(Replicated)
	TArray<FReplicatedPlayerMatchInfo> PlayerStates;

protected:
	UFUNCTION()
	void OnRep_MatchPhase();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

};

