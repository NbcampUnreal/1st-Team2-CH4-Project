// GM_MainMode.cpp

#include "GM_MainMode.h"


void AGM_MainMode::BeginPlay()
{
	Super::BeginPlay();

	// GameState를 가져와서 모드 선택 UI 표시 상태로 전환
	
	AGS_FighterState* GS = GetGameState<AGS_FighterState>();
	if (GS)
	{
		GS->bShowModeSelectUI = true;
		UE_LOG(LogTemp, Log, TEXT("MainMode: Show Mode Select UI"));
	}
	
}