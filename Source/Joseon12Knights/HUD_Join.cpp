#include "HUD_Join.h"
#include "Components/VerticalBox.h"
#include "WBP_ServerButton.h"

void UHUD_Join::NativeConstruct()
{
    Super::NativeConstruct();
}

void UHUD_Join::PopulateRoomList(const TArray<FOnlineSessionSearchResult>& Results)
{
    if (!RoomListBox || !ServerButtonClass) return;

    RoomListBox->ClearChildren();

    for (const auto& Result : Results)
    {
        if (!Result.IsValid()) continue;

        UWBP_ServerButton* Button = CreateWidget<UWBP_ServerButton>(this, ServerButtonClass);
        if (!Button) continue;

        Button->Setup(Result); 
        RoomListBox->AddChild(Button);
    }
}

