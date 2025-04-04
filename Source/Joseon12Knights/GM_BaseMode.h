// GM_BaseMode.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GI_GameCoreInstance.h"
#include "GS_FighterState.h"
#include "GM_BaseMode.generated.h"

UCLASS()
class JOSEON12KNIGHTS_API AGM_BaseMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	// 캐릭터 선택 UI 시작
	virtual void StartCharacterSelect();

	// 캐릭터 선택 완료 시 호출
	virtual void OnCharacterSelected(APlayerController* PC, const FString& CharacterID);

	// 맵 선택 UI 시작
	virtual void StartMapSelect();

	// 맵 선택 완료 시 호출
	virtual void OnMapSelected(const FString& MapName);

	// 셀렉 완료 후 다음 단계로 진행
	virtual void ProceedToMatch();

protected:
	// GameInstance 캐스팅 함수
	UGI_GameCoreInstance* GetGI() const;

	// GameState 캐스팅 함수
	AGS_FighterState* GetGS() const;
};
