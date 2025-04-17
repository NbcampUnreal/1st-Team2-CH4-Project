// GM_BaseMode.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GI_GameCoreInstance.h"
#include "GS_FighterState.h"
#include "characters/MainPlayerState.h"
#include "GM_BaseMode.generated.h"

UCLASS()
class JOSEON12KNIGHTS_API AGM_BaseMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void StartCharacterSelect();

	virtual void OnCharacterSelected(APlayerController* PC, const FString& CharacterID);

	virtual void StartMapSelect();

	virtual void OnMapSelected(const FString& MapName);

	virtual void ProceedToMatch();

	virtual void HandlePlayerRespawn(AActor* PlayerActor);

	virtual bool CanRespawn(AActor* PlayerActor) const;

protected:
	UGI_GameCoreInstance* GetGI() const;

	AGS_FighterState* GetGS() const;
};
