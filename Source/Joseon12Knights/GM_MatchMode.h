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
	virtual void PostLogin(APlayerController* NewPlayer) override;

public:
	UPROPERTY(EditAnywhere, Category = "Characters")
	TMap<FString, TSubclassOf<APawn>> CharacterBPMap;

	UPROPERTY(EditAnywhere, Category = "Characters")
	TMap<int32, APawn*> SpawnedCharacters;
};
