#pragma once

#include "CoreMinimal.h"
#include "GM_BaseMode.h"        
#include "characters/MainPlayerState.h"
#include "RespawnManagerInterface.h"
#include "GM_MatchMode.generated.h"

UCLASS()
class JOSEON12KNIGHTS_API AGM_MatchMode : public AGM_BaseMode 
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	virtual void PostLogin(APlayerController* NewPlayer) override;

public:
	virtual void HandlePlayerRespawn(AActor* PlayerActor) override; 
	virtual bool CanRespawn(AActor* PlayerActor) const override;


	UPROPERTY(EditAnywhere, Category = "Characters")
	TMap<FString, TSubclassOf<APawn>> CharacterBPMap;

	UPROPERTY(EditAnywhere, Category = "Characters")
	TMap<int32, APawn*> SpawnedCharacters;
};
