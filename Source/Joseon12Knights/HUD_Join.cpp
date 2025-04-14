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

        // 예: Tile에 텍스트 설정 로직이 있다면 여기에 추가
        // e.g., Cast<UJoinRoomTile>(RoomTile)->Setup(RoomName);

        RoomListBox->AddChild(RoomTile);
    }
}
