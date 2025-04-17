#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUD_CharacterInformation.generated.h"

class UTextBlock;
class UProgressBar;
class UImage;
class UTexture2D;


UCLASS()
class JOSEON12KNIGHTS_API UHUD_CharacterInformation : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

protected:
	void UpdateHUD();

	UPROPERTY(meta = (BindWidget))
	UTextBlock* StockBox;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* CharacterNameText;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* SuperBar;

	UPROPERTY(meta = (BindWidget))
	UImage* Portrate;

	UPROPERTY(meta = (BindWidget))
	UImage* SkillIcon_W;

	UPROPERTY(meta = (BindWidget))
	UImage* SkillIcon_R;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Icons")
	TMap<FString, UTexture2D*> CharacterIconMap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Icons")
	TMap<FString, UTexture2D*> SkillIconWMap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Icons")
	TMap<FString, UTexture2D*> SkillIconRMap;


};
