// PC_MenuController.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GS_FighterState.h"
#include "GM_SingleMode.h"
#include "GM_LobbyMode.h"
#include "CP_UIController.h"
#include "HUD_MapSelect.h"
#include "HUD_PressStart.h"
#include "HUD_CharacterSelect.h"
#include "HUD_CharacterStory.h"
#include "GI_GameCoreInstance.h"
#include "PS_FighterPlayerState.h"
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

	UFUNCTION(BlueprintCallable)
	void SelectVS();

	UFUNCTION(BlueprintCallable)
	void SelectOnline();

	UFUNCTION(Server, Reliable)
	void Server_SetReady();

	UFUNCTION(BlueprintCallable)
	void SelectJoin();

	UFUNCTION(BlueprintCallable)
	void SelectArcade();

	UFUNCTION(BlueprintCallable)
	void OnCharacterSelectConfirmed(int32 NumAI);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void SetupInputComponent() override;

	void OnGameStartPressed();
	void OnPressStartPressed();
	void OnCharacterSelectEnterPressed();
	void OnCharacterStoryEnterPressed();

	AGS_FighterState* GetGS() const;
	UGI_GameCoreInstance* GetGI() const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UCP_UIController> UIControllerClass;

	UPROPERTY()
	UCP_UIController* UIController;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> JoinMenuClass;

	UPROPERTY()
	UUserWidget* JoinMenuWidget;

};
