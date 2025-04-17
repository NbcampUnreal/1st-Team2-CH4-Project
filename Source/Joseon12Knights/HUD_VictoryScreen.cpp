// HUD_VictoryScreen.cpp

#include "HUD_VictoryScreen.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"

void UHUD_VictoryScreen::NativeConstruct()
{
	Super::NativeConstruct();

	// 엔터 키 입력 바인딩
	if (APlayerController* PC = GetOwningPlayer())
	{
		if (PC->InputComponent)
		{
			PC->InputComponent->BindAction("GameStart", IE_Pressed, this, &UHUD_VictoryScreen::OnEnterPressed);
		}
	}
}


void UHUD_VictoryScreen::SetVictoryInfo(int32 WinnerIndex, const FString& WinnerCharName)
{
	int32 MyIndex = 0;
	if (const APlayerState* PS = GetOwningPlayerState())
	{
		const AMainPlayerState* MPS = Cast<AMainPlayerState>(PS);
		if (MPS)
		{
			MyIndex = MPS->GetPlayerIndex();
		}
	}

	bool bIsWinner = (MyIndex == WinnerIndex);

	if (WinnerText)
	{
		FString Result = bIsWinner
			? FString::Printf(TEXT("승리했습니다!"))
			: FString::Printf(TEXT("패배했습니다!"));

		WinnerText->SetText(FText::FromString(Result));
	}

	if (CharName)
	{
		CharName->SetText(FText::FromString(WinnerCharName));
	}
}


void UHUD_VictoryScreen::OnEnterPressed()
{
	UGameplayStatics::OpenLevel(this, FName("Menus"));
}
