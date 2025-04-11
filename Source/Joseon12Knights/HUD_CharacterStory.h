#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUD_CharacterStory.generated.h"

UCLASS()
class JOSEON12KNIGHTS_API UHUD_CharacterStory : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UPROPERTY()
	TArray<class UHUD_CharacterSelectTile*> Tiles;

	UPROPERTY(meta = (BindWidget)) class UHUD_CharacterSelectTile* CharacterPannel_1;
	UPROPERTY(meta = (BindWidget)) class UHUD_CharacterSelectTile* CharacterPannel_2;
	UPROPERTY(meta = (BindWidget)) class UHUD_CharacterSelectTile* CharacterPannel_3;
	UPROPERTY(meta = (BindWidget)) class UHUD_CharacterSelectTile* CharacterPannel_4;
	UPROPERTY(meta = (BindWidget)) class UHUD_CharacterSelectTile* CharacterPannel_5;
	UPROPERTY(meta = (BindWidget)) class UHUD_CharacterSelectTile* CharacterPannel_6;
	UPROPERTY(meta = (BindWidget)) class UHUD_CharacterSelectTile* CharacterPannel_7;
	UPROPERTY(meta = (BindWidget)) class UHUD_CharacterSelectTile* CharacterPannel_8;

	UPROPERTY(meta = (BindWidget)) class UHUD_CharacterSelectPanel* Panel1;

	UPROPERTY()
	FString SelectedCharacterID;

	void NotifyCharacterSelected(const FString& CharacterID, int32 PlayerIndex);


	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* PressStartAnim;

	UFUNCTION()
	void ConfirmSelection();

	UPROPERTY(meta = (BindWidget)) class UButton* BackButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map")
	TSoftObjectPtr<UWorld> StoryMapAsset;

private:

	UFUNCTION()
	void HandleBack();
};
