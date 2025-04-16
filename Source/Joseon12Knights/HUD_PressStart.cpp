#include "HUD_PressStart.h"
#include "PC_MenuController.h"

void UHUD_PressStart::NativeConstruct()
{
	Super::NativeConstruct();


	if (Flash)
	{
		PlayAnimation(Flash, 0.f, 0); 
	}

}

void UHUD_PressStart::SimulateStartPress()
{
	if (APlayerController* PC = GetOwningPlayer())
	{
		if (APC_MenuController* MenuPC = Cast<APC_MenuController>(PC))
		{
			MenuPC->HandleBackToMainMenu();
		}
	}
}
