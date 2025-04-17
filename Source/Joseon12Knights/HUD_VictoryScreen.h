// HUD_VictoryScreen.h

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainPlayerState.h"
#include "HUD_VictoryScreen.generated.h"

class UTextBlock;

UCLASS()
class JOSEON12KNIGHTS_API UHUD_VictoryScreen : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	void SetVictoryInfo(int32 PlayerIndex, const FString& CharacterName);
	
protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* WinnerText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* CharName;

	UFUNCTION()
	void OnEnterPressed();
};
