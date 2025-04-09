#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUD_CharacterSelectPanel.generated.h"

UCLASS()
class JOSEON12KNIGHTS_API UHUD_CharacterSelectPanel : public UUserWidget
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable)
    void SetCharacter(const FString& CharacterID, const FString& PlayerID);

protected:
    UPROPERTY(meta = (BindWidget))
    class UImage* CharacterImage;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* Name;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* Player;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character")
    TMap<FString, UTexture2D*> CharacterIconMap;
};

