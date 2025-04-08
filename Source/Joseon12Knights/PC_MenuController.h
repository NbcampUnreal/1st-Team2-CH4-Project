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

	// 상태 감지 함수
	void CheckAndShowUI();
	void OnPressStart();

	// 한 번만 띄우도록 체크 변수
	bool bModeUIShown = false;
	bool bCharacterUIShown = false;
	bool bPressStartUIShown = false;

	// GameState / GameInstance 캐스팅 헬퍼
	AGS_FighterState* GetGS() const;
	UGI_GameCoreInstance* GetGI() const;

	// 위젯 클래스들 (위젯 연결은 블루프린트에서 설정)
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
	void OnCharacterSelectConfirmed(int32 NumAI); // 싱글이면 AI 수 전달, 온라인이면 무시


	// 실제 생성된 위젯
	UUserWidget* CurrentWidget;
};
