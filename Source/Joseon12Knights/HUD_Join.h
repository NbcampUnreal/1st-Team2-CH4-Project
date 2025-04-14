#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUD_Join.generated.h"

class UVerticalBox;

UCLASS()
class JOSEON12KNIGHTS_API UHUD_Join : public UUserWidget
{
    GENERATED_BODY()

public:
    virtual void NativeConstruct() override;

    // 추후 세션 정보를 리스트로 띄울 함수
    void PopulateRoomList(const TArray<FString>& RoomNames);  // 임시로 FString

protected:
    // UMG에서 VerticalBox 바인딩 (Room 리스트용)
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    UVerticalBox* RoomListBox;


    // 세션 아이템으로 사용할 위젯 클래스 (JoinRoomTile)
    UPROPERTY(EditDefaultsOnly, Category = "Join")
    TSubclassOf<UUserWidget> RoomTileClass;
};
