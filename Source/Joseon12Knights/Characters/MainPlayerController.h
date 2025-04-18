#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
class UHUD_VictoryScreen;

UCLASS()
class JOSEON12KNIGHTS_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AMainPlayerController();

	// Input
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputMappingContext* InputMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* NormalAttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* SkillAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* UltimateAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* RollAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* GuardAction;

	// Character Info HUD
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	TSubclassOf<UUserWidget> CharacterInfoWidgetClass;

	UPROPERTY()
	UUserWidget* CharacterInfoWidget;

	// Victory HUD
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	TSubclassOf<UUserWidget> VictoryScreenClass;

	UPROPERTY()
	UUserWidget* VictoryScreenWidget;

	// 승리 메시지 전달
	UFUNCTION(Client, Reliable)
	void ClientReceiveVictory(int32 PlayerIndex, const FString& CharName);

protected:
	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;

	void HandleVictoryEnterKey();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* ResetPressedAction;


};
