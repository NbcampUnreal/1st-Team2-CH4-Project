#include "HUD_ReadyText.h"
#include "Components/Widget.h"

void UHUD_ReadyText::NativeConstruct()
{
    Super::NativeConstruct();
    // 초기화 필요 시 여기에
}

void UHUD_ReadyText::ShowReadyForPlayer(int32 Index)
{
    UE_LOG(LogTemp, Warning, TEXT("▶ ShowReadyForPlayer(%d) 호출됨"), Index);

    switch (Index)
    {
    case 0:
        if (ready_1) {
            ready_1->SetVisibility(ESlateVisibility::Visible);
            UE_LOG(LogTemp, Warning, TEXT("✅ ready_1 표시됨"));
        }
        else {
            UE_LOG(LogTemp, Error, TEXT("❌ ready_1 위젯 null"));
        }
        break;
    case 1:
        if (ready_2) {
            ready_2->SetVisibility(ESlateVisibility::Visible);
            UE_LOG(LogTemp, Warning, TEXT("✅ ready_2 표시됨"));
        }
        else {
            UE_LOG(LogTemp, Error, TEXT("❌ ready_2 위젯 null"));
        }
        break;
    case 2:
        if (ready_3) {
            ready_3->SetVisibility(ESlateVisibility::Visible);
            UE_LOG(LogTemp, Warning, TEXT("✅ ready_3 표시됨"));
        }
        else {
            UE_LOG(LogTemp, Error, TEXT("❌ ready_3 위젯 null"));
        }
        break;
    case 3:
        if (ready_4) {
            ready_4->SetVisibility(ESlateVisibility::Visible);
            UE_LOG(LogTemp, Warning, TEXT("✅ ready_4 표시됨"));
        }
        else {
            UE_LOG(LogTemp, Error, TEXT("❌ ready_4 위젯 null"));
        }
        break;
    default:
        UE_LOG(LogTemp, Error, TEXT("❌ Index 범위 초과: %d"), Index);
        break;
    }
}


