// PC_MenuController.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GS_FighterState.h"
#include "GM_SingleMode.h"
#include "GM_LobbyMode.h"
#include "GI_GameCoreInstance.h"
#include "PC_MenuController.generated.h"

UCLASS()
class JOSEON12KNIGHTS_API APC_MenuController : public APlayerController
{
	GENERATED_BODY()

public:
	void HandleBackToMainMenu();
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void SetupInputComponent() override;

	// ���� ���� �Լ�
	void CheckAndShowUI();
	void OnPressStart();

	// �� ���� ��쵵�� üũ ����
	bool bModeUIShown = false;
	bool bCharacterUIShown = false;
	bool bPressStartUIShown = false;

	// GameState / GameInstance ĳ���� ����
	AGS_FighterState* GetGS() const;
	UGI_GameCoreInstance* GetGI() const;

	// ���� Ŭ������ (���� ������ �������Ʈ���� ����)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> PressStartWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> ModeSelectWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> CharacterSelectWidgetClass;

	UFUNCTION(BlueprintCallable)
	void SelectCharacter(const FString& CharacterID);

	UFUNCTION(BlueprintCallable)
	void SelectVS();

	UFUNCTION(BlueprintCallable)
	void OnCharacterSelectConfirmed(int32 NumAI); // �̱��̸� AI �� ����, �¶����̸� ����


	// ���� ������ ����
	UUserWidget* CurrentWidget;
};
