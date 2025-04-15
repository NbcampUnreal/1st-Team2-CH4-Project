// PS_FighterPlayerState.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "PS_FighterPlayerState.generated.h"

UCLASS()
class JOSEON12KNIGHTS_API APS_FighterPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	APS_FighterPlayerState();

	UPROPERTY(Replicated, BlueprintReadOnly)
	bool bIsReady = false;

	UPROPERTY(Replicated, BlueprintReadWrite)
	FString SelectedCharacterID;


protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
