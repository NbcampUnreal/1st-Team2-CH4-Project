// GM_MatchMode.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GM_MatchMode.generated.h"

UCLASS()
class JOSEON12KNIGHTS_API AGM_MatchMode : public AGameModeBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	// 캐릭터 스폰 함수
	void SpawnCharacters();

	// 점수/낙사 처리 등 전투 중 로직은 추후 추가 예정
};
