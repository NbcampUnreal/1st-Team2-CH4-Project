// PS_FighterPlayerState.cpp
#include "PS_FighterPlayerState.h"
#include "Net/UnrealNetwork.h"

APS_FighterPlayerState::APS_FighterPlayerState()
{
	bIsReady = false;
	bReplicates = true;
}

void APS_FighterPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(APS_FighterPlayerState, bIsReady);
    DOREPLIFETIME(APS_FighterPlayerState, SelectedCharacterID);
}

