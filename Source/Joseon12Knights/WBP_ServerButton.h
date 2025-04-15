#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "WBP_ServerButton.generated.h"

UCLASS()
class JOSEON12KNIGHTS_API UWBP_ServerButton : public UUserWidget
{
	GENERATED_BODY()

public:
	void Setup(const FOnlineSessionSearchResult& InResult);

protected:
	virtual void NativeConstruct() override;

	// 버튼 클릭 시 세션 참여
	UFUNCTION()
	void OnJoinClicked();

	UPROPERTY(meta = (BindWidget))
	class UButton* JoinButton;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* SessionNameText;

private:
	FOnlineSessionSearchResult Result;
};
