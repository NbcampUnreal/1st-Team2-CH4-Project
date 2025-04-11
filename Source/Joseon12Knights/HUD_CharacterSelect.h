#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GS_FighterState.h" 
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "HUD_CharacterSelect.generated.h"

class UHUD_CharacterSelectTile;
class UHUD_CharacterSelectPanel;
class UButton;
class APC_MenuController;

UCLASS()
class JOSEON12KNIGHTS_API UHUD_CharacterSelect : public UUserWidget
{
    GENERATED_BODY()

public:
    virtual void NativeConstruct() override;

    void NotifyCharacterSelected(const FString& CharacterID, int32 PlayerIndex);

    void ClearCpuCharacterFromPanel(int32 CpuIndex);

protected:

    UPROPERTY(meta = (BindWidgetAnim), Transient)
    UWidgetAnimation* PressStartAnim;

    bool bPressStartPlayed = false;

    TMap<FString, TArray<int32>> CharacterSelections;


    UPROPERTY(meta = (BindWidget)) UTextBlock* PressStart;

    UPROPERTY(meta = (BindWidget)) UHUD_CharacterSelectTile* CharacterPannel_1;
    UPROPERTY(meta = (BindWidget)) UHUD_CharacterSelectTile* CharacterPannel_2;
    UPROPERTY(meta = (BindWidget)) UHUD_CharacterSelectTile* CharacterPannel_3;
    UPROPERTY(meta = (BindWidget)) UHUD_CharacterSelectTile* CharacterPannel_4;
    UPROPERTY(meta = (BindWidget)) UHUD_CharacterSelectTile* CharacterPannel_5;
    UPROPERTY(meta = (BindWidget)) UHUD_CharacterSelectTile* CharacterPannel_6;
    UPROPERTY(meta = (BindWidget)) UHUD_CharacterSelectTile* CharacterPannel_7;
    UPROPERTY(meta = (BindWidget)) UHUD_CharacterSelectTile* CharacterPannel_8;

    UPROPERTY(meta = (BindWidget)) UHUD_CharacterSelectPanel* Pannel1;
    UPROPERTY(meta = (BindWidget)) UHUD_CharacterSelectPanel* Pannel2;
    UPROPERTY(meta = (BindWidget)) UHUD_CharacterSelectPanel* Pannel3;
    UPROPERTY(meta = (BindWidget)) UHUD_CharacterSelectPanel* Pannel4;

    UPROPERTY(meta = (BindWidget)) UButton* BackButton;
    
    UPROPERTY()
    TArray<UHUD_CharacterSelectTile*> Tiles;

    // 현재 AI 수
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
    int32 CpuCount = 0;

    // 최대 허용 가능한 AI 수 (4 - 실제 플레이어 수)
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
    int32 MaxCpuCount = 0;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* CPU;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* Hard;


    UFUNCTION(BlueprintCallable)
    void OnCpuUp();

    UFUNCTION(BlueprintCallable)
    void OnCpuDown();

    UFUNCTION(BlueprintCallable)
    void UpdateCpuText();

    UFUNCTION(BlueprintCallable)
    void ApplyCpuCharacters();

    void RecalculateMaxCpu();

    FString GetAvailableRandomID();

    void ApplyCpuCharacterToPanel(int32 CpuIndex, const FString& ID);

    UFUNCTION()
    void OnBackClicked();
};
