// GM_MainMode.cpp

#include "GM_MainMode.h"


void AGM_MainMode::BeginPlay()
{
	Super::BeginPlay();

	// GameState�� �����ͼ� ��� ���� UI ǥ�� ���·� ��ȯ
	
	AGS_FighterState* GS = GetGameState<AGS_FighterState>();
	if (GS)
	{
		GS->bShowModeSelectUI = true;
		UE_LOG(LogTemp, Log, TEXT("MainMode: Show Mode Select UI"));
	}
	
}