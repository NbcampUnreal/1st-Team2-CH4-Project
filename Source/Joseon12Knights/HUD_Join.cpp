#include "HUD_Join.h"
#include "Components/VerticalBox.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"

void UHUD_Join::NativeConstruct()
{
    Super::NativeConstruct();

    // 기본 리스트 초기화 등
}

void UHUD_Join::PopulateRoomList(const TArray<FString>& RoomNames)
{
    if (!RoomListBox || !RoomTileClass) return;

    RoomListBox->ClearChildren();

    for (const FString& RoomName : RoomNames)
    {
        UUserWidget* RoomTile = CreateWidget<UUserWidget>(this, RoomTileClass);
        if (!RoomTile) continue;

        RoomListBox->AddChild(RoomTile);
    }
}
