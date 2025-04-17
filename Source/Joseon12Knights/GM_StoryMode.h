#pragma once

#include "CoreMinimal.h"
#include "GM_BaseMode.h"
#include "GM_StoryMode.generated.h"

UCLASS()
class JOSEON12KNIGHTS_API AGM_StoryMode : public AGM_BaseMode
{
	GENERATED_BODY()

public:
	AGM_StoryMode();

	virtual void BeginPlay() override;
	virtual void HandlePlayerRespawn(AActor* PlayerActor) override;
	virtual bool CanRespawn(AActor* PlayerActor) const override;

protected:
	// 선택된 캐릭터로 게임 시작


	void TriggerStoryGameEnd(bool bPlayerWin);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character")
	TMap<FString, TSubclassOf<APawn>> CharacterBPMap;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character")
	TMap<FString, TSubclassOf<APawn>> CharacterBPMap_AI;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Map")
	TSoftObjectPtr<UWorld> StoryMapAsset;


	int32 TotalAI = 1;
	int32 DeadAI = 0;
};
