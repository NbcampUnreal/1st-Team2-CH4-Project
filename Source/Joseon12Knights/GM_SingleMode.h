#pragma once

#include "CoreMinimal.h"
#include "GM_BaseMode.h"
#include "GM_SingleMode.generated.h"

UCLASS()
class JOSEON12KNIGHTS_API AGM_SingleMode : public AGM_BaseMode
{
	GENERATED_BODY()

public:
	AGM_SingleMode();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	// 리스폰 처리
	void HandlePlayerRespawn(AActor* PlayerActor);
	bool CanRespawn(AActor* PlayerActor) const;

	// 게임 종료
	void TriggerSingleGameEnd(bool bPlayerWin);

private:
	void SpawnCharacterAtTag(const FString& Tag, const FString& CharID, bool bIsPlayer, int32 Index);

private:
	bool bGameStarted = false;
	bool bGameEnded = false;

	UPROPERTY(EditAnywhere, Category = "Characters")
	TMap<FString, TSubclassOf<APawn>> CharacterBPMap;

	UPROPERTY(EditAnywhere, Category = "Characters")
	TMap<FString, TSubclassOf<APawn>> CharacterBPMap_AI;


	int32 TotalAI = 0;
	int32 DeadAI = 0;
};
