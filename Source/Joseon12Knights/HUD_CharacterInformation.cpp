// HUD_CharacterInformation.cpp
#include "HUD_CharacterInformation.h"
#include "Kismet/GameplayStatics.h"
#include "GI_GameCoreInstance.h"
#include "Characters/MainPlayerState.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"
#include "GameFramework/PlayerController.h"

void UHUD_CharacterInformation::NativeConstruct()
{
	Super::NativeConstruct();
	UpdateHUD();
}

void UHUD_CharacterInformation::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	UpdateHUD();
}

void UHUD_CharacterInformation::UpdateHUD()
{
	APlayerController* PC = GetOwningPlayer();
	if (!PC)
	{
		return;
	}

	AMainPlayerState* PS = Cast<AMainPlayerState>(PC->PlayerState);
	if (!PS)
	{
		return;
	}

	UGI_GameCoreInstance* GI = Cast<UGI_GameCoreInstance>(UGameplayStatics::GetGameInstance(this));
	if (!GI)
	{
		return;
	}

	if (StockBox)
	{
		int32 Stock = PS->GetStock();
		if (Stock >= 0 && Stock <= 10) 
		{
			FString StockStr = FString::Printf(TEXT("%d"), Stock);
			StockBox->SetText(FText::FromString(StockStr));
		}
	}



	if (CharacterNameText)
	{
		FString ID = GI->SelectedCharacterID;
		FString Name;

		if (ID == "1") Name = TEXT("자 기사");
		else if (ID == "2") Name = TEXT("축 기사");
		else if (ID == "3") Name = TEXT("인 기사");
		else if (ID == "4") Name = TEXT("사 기사");
		else if (ID == "5") Name = TEXT("Mage");
		else if (ID == "6") Name = TEXT("^v^");
		else Name = TEXT("Unknown");

		CharacterNameText->SetText(FText::FromString(Name));

	}


	if (SuperBar && PS->GetMaxHealth() > 0)
	{
		float MaxHP = PS->GetMaxHealth();
		float CurrHP = PS->GetCurrentHealth();

		float VisualPercent = FMath::Clamp((CurrHP / MaxHP) * 0.5f, 0.f, 1.f);

		SuperBar->SetPercent(VisualPercent);

	}




	if (Portrate && GI && CharacterIconMap.Contains(GI->SelectedCharacterID))
	{
		UTexture2D* Texture = CharacterIconMap[GI->SelectedCharacterID];
		if (Texture)
		{
			FSlateBrush Brush;
			Brush.SetResourceObject(Texture);
			Portrate->SetBrush(Brush);
			
		}
	}


	if (SkillIcon_W && SkillIconWMap.Contains(GI->SelectedCharacterID))
	{
		UTexture2D* TextureW = SkillIconWMap[GI->SelectedCharacterID];
		if (TextureW)
		{
			FSlateBrush Brush;
			Brush.SetResourceObject(TextureW);
			SkillIcon_W->SetBrush(Brush);

		}
	}


	if (SkillIcon_R && SkillIconRMap.Contains(GI->SelectedCharacterID))
	{
		UTexture2D* TextureR = SkillIconRMap[GI->SelectedCharacterID];
		if (TextureR)
		{
			FSlateBrush Brush;
			Brush.SetResourceObject(TextureR);
			SkillIcon_R->SetBrush(Brush);
			
		}
	}




}
