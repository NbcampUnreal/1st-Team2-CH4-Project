// PC_MenuController.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GS_FighterState.h"
#include "GM_SingleMode.h"
#include "GM_LobbyMode.h"
#include "HUD_MapSelect.h"
#include "GI_GameCoreInstance.h"
#include "PC_MenuController.generated.h"

UCLASS()
class JOSEON12KNIGHTS_API APC_MenuController : public APlayerController
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void HandleBackToCharacterSelect();

	UFUNCTION(BlueprintCallable)
	void HandleBackToMainMenu();

	UFUNCTION(BlueprintCallable)
	void SelectCharacter(const FString& CharacterID);

	AGS_FighterState* GetGS() const;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void SetupInputComponent() override;

	void CheckAndShowUI();
	void OnPressStart();

	UFUNCTION()
	void OnConfirmPressed();

	UFUNCTION()
	void OnGameStartPressed();

	bool bModeUIShown = false;
	bool bCharacterUIShown = false;
	bool bPressStartUIShown = false;


	UGI_GameCoreInstance* GetGI() const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> PressStartWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> ModeSelectWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> CharacterSelectWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> MapSelectWidgetClass;

	UFUNCTION(BlueprintCallable)
	void SelectVS();

	UFUNCTION(BlueprintCallable)
	void OnCharacterSelectConfirmed(int32 NumAI);


	UUserWidget* CurrentWidget;
};
