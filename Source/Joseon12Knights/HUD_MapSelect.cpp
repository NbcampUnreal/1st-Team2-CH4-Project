#include "HUD_MapSelect.h"
#include "Components/Button.h"
#include "PC_MenuController.h"

void UHUD_MapSelect::NativeConstruct()
{
    Super::NativeConstruct();

    if (BackButton)
    {
        BackButton->OnClicked.AddDynamic(this, &UHUD_MapSelect::OnBackClicked);
    }
}

void UHUD_MapSelect::OnBackClicked()
{
    if (APlayerController* PC = GetOwningPlayer())
    {
        if (APC_MenuController* MenuPC = Cast<APC_MenuController>(PC))
        {
            MenuPC->HandleBackToCharacterSelect(); 
        }
    }
}
