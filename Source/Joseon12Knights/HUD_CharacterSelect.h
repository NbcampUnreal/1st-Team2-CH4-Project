#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUD_CharacterSelect.generated.h"

class UHUD_CharacterSelectTile;
class UHUD_CharacterSelectPanel;
class UButton;

UCLASS()
class JOSEON12KNIGHTS_API UHUD_CharacterSelect : public UUserWidget
{
    GENERATED_BODY()

public:
    virtual void NativeConstruct() override;
  
    
protected:
    // Ÿ�ϵ� (���� 8ĭ - �������Ʈ �̸� ����)
    UPROPERTY(meta = (BindWidget)) UHUD_CharacterSelectTile* CharacterPannel_1;
    UPROPERTY(meta = (BindWidget)) UHUD_CharacterSelectTile* CharacterPannel_2;
    UPROPERTY(meta = (BindWidget)) UHUD_CharacterSelectTile* CharacterPannel_3;
    UPROPERTY(meta = (BindWidget)) UHUD_CharacterSelectTile* CharacterPannel_4;
    UPROPERTY(meta = (BindWidget)) UHUD_CharacterSelectTile* CharacterPannel_5;
    UPROPERTY(meta = (BindWidget)) UHUD_CharacterSelectTile* CharacterPannel_6;
    UPROPERTY(meta = (BindWidget)) UHUD_CharacterSelectTile* CharacterPannel_7;
    UPROPERTY(meta = (BindWidget)) UHUD_CharacterSelectTile* CharacterPannel_8;

    // �гε� (�Ʒ��� 4ĭ)
    
    UPROPERTY(meta = (BindWidget)) UHUD_CharacterSelectPanel* Pannel1;

    UPROPERTY(meta = (BindWidget)) UHUD_CharacterSelectPanel* Pannel2;
    UPROPERTY(meta = (BindWidget)) UHUD_CharacterSelectPanel* Pannel3;
    UPROPERTY(meta = (BindWidget)) UHUD_CharacterSelectPanel* Pannel4;

    UPROPERTY(meta = (BindWidget)) UButton* BackButton;

    UFUNCTION()
    void OnBackClicked();
};
