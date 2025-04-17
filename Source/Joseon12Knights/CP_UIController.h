#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CP_UIController.generated.h"

UENUM(BlueprintType)
enum class EUIScreen : uint8
{
	PressStart,
	MainMenu,
	CharacterSelect,
	MapSelect,
	CharacterStory,
	Online,
	Join,
	Result
};

/**
 * UI 전환을 통합 관리하는 클래스
 */
UCLASS(Blueprintable)
class JOSEON12KNIGHTS_API UCP_UIController : public UObject
{
	GENERATED_BODY()

public:
	UCP_UIController(); 

	void Initialize(APlayerController* InOwner);
	void ShowUI(EUIScreen TargetScreen);

	UUserWidget* GetCurrentWidget() const { return CurrentWidget; }


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> PressStartClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> MainMenuClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> CharacterSelectClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> MapSelectClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> CharacterStoryClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> OnlineClass; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> JoinMenuClass;


private:
	UPROPERTY()
	APlayerController* OwningPC;

	UPROPERTY()
	UUserWidget* CurrentWidget;



};

