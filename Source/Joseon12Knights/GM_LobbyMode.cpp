// GM_LobbyMode.cpp
#include "GM_LobbyMode.h"
#include "GameFramework/PlayerState.h"
#include "GS_FighterState.h"
#include "GI_GameCoreInstance.h"
#include "PC_MenuController.h"
#include "PS_FighterPlayerState.h"

void AGM_LobbyMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	UE_LOG(LogTemp, Log, TEXT("Player joined lobby: %s"), *NewPlayer->GetName());
	CheckAllPlayersReady();
}

void AGM_LobbyMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	UE_LOG(LogTemp, Log, TEXT("Player left lobby: %s"), *Exiting->GetName());
	CheckAllPlayersReady();
}

void AGM_LobbyMode::CheckAllPlayersReady()
{
	AGS_FighterState* GS = GetGameState<AGS_FighterState>();
	if (!GS) return;

	for (APlayerState* PS : GS->PlayerArray)
	{
		APS_FighterPlayerState* FighterPS = Cast<APS_FighterPlayerState>(PS);
		if (!FighterPS || !FighterPS->bIsReady)
		{
			UE_LOG(LogTemp, Log, TEXT("Player not ready: %s"), PS ? *PS->GetName() : TEXT("NULL"));
			return;
		}
	}

	UE_LOG(LogTemp, Log, TEXT("All players ready. Starting character select."));
	StartCharacterSelect();
}

void AGM_LobbyMode::Server_SetReady_Implementation(APlayerController* PC)
{
	if (PC && PC->PlayerState)
	{
		if (APS_FighterPlayerState* FighterPS = Cast<APS_FighterPlayerState>(PC->PlayerState))
		{
			FighterPS->bIsReady = true;
			UE_LOG(LogTemp, Log, TEXT("Player set ready: %s"), *PC->GetName());
			CheckAllPlayersReady();
		}
	}
}

