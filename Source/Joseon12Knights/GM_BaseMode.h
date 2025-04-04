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
	// ĳ���� ���� UI ����
	virtual void StartCharacterSelect();

	// ĳ���� ���� �Ϸ� �� ȣ��
	virtual void OnCharacterSelected(APlayerController* PC, const FString& CharacterID);

	// �� ���� UI ����
	virtual void StartMapSelect();

	// �� ���� �Ϸ� �� ȣ��
	virtual void OnMapSelected(const FString& MapName);

	// ���� �Ϸ� �� ���� �ܰ�� ����
	virtual void ProceedToMatch();

protected:
	// GameInstance ĳ���� �Լ�
	UGI_GameCoreInstance* GetGI() const;

	// GameState ĳ���� �Լ�
	AGS_FighterState* GetGS() const;
};
