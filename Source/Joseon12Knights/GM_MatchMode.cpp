// GM_MatchMode.cpp
#include "GM_MatchMode.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerState.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "GI_GameCoreInstance.h"
#include "PS_FighterPlayerState.h"

void AGM_MatchMode::BeginPlay()
{
	Super::BeginPlay();

	SpawnCharacters();
}

void AGM_MatchMode::SpawnCharacters()
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		APlayerController* PC = It->Get();
		if (!PC) continue;

		APS_FighterPlayerState* FighterPS = Cast<APS_FighterPlayerState>(PC->PlayerState);
		FString CharacterID = FighterPS ? FighterPS->GetPlayerName() : TEXT("DefaultChar"); // �ӽ� ID
	}
}
