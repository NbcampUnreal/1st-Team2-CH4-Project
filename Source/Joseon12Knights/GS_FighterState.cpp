#include "GS_FighterState.h"
#include "Net/UnrealNetwork.h"

AGS_FighterState::AGS_FighterState()
{
	bReplicates = true;
	MatchPhase = EMatchPhase::Lobby;
	bShowCharacterSelect = false;
	bShowModeSelectUI = false;
}

void AGS_FighterState::OnRep_MatchPhase()
{
	UE_LOG(LogTemp, Log, TEXT("MatchPhase updated: %d"), static_cast<uint8>(MatchPhase));
}

void AGS_FighterState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGS_FighterState, MatchPhase);
	DOREPLIFETIME(AGS_FighterState, bShowCharacterSelect);
	DOREPLIFETIME(AGS_FighterState, bShowModeSelectUI);
	DOREPLIFETIME(AGS_FighterState, PlayerStates);
}