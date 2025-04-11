#include "HUD_PressStart.h"
#include "PC_MenuController.h"

void UHUD_PressStart::NativeConstruct()
{
	Super::NativeConstruct();


	if (Flash)
	{
		PlayAnimation(Flash, 0.f, 0); // 0이면 무한 반복
	}

}

void UHUD_PressStart::SimulateStartPress()
{
	if (APlayerController* PC = GetOwningPlayer())
	{
		if (APC_MenuController* MenuPC = Cast<APC_MenuController>(PC))
		{
			UE_LOG(LogTemp, Warning, TEXT("📢 SimulateStartPress 호출됨"));

			MenuPC->HandleBackToMainMenu();
		}
	}
}
