#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OnlineSessionSettings.h"
#include "WBP_ServerButton.h"
#include "HUD_Join.generated.h"

class UVerticalBox;

UCLASS()
class JOSEON12KNIGHTS_API UHUD_Join : public UUserWidget
{
    GENERATED_BODY()

public:
    virtual void NativeConstruct() override;

    void PopulateRoomList(const TArray<FOnlineSessionSearchResult>& Results);

protected:
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    UVerticalBox* RoomListBox;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Join")
    TSubclassOf<UWBP_ServerButton> ServerButtonClass;
};
