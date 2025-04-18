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
	virtual void HandlePlayerRespawn(AActor* PlayerActor) override;
	virtual bool CanRespawn(AActor* PlayerActor) const override;

protected:
	void TriggerSingleGameEnd(bool bPlayerWin);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character")
	TMap<FString, TSubclassOf<APawn>> CharacterBPMap;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character")
	TMap<FString, TSubclassOf<APawn>> CharacterBPMap_AI;

	// AI 개수 추적용
	int32 TotalAI = 0;
	int32 DeadAI = 0;
};
