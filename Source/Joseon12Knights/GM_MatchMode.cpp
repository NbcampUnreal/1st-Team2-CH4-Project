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

	UE_LOG(LogTemp, Log, TEXT("MatchMode: BeginPlay - Spawning characters"));
	SpawnCharacters();
}

void AGM_MatchMode::SpawnCharacters()
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		APlayerController* PC = It->Get();
		if (!PC) continue;

		// 캐릭터 ID 가져오기 (PlayerState 기준 예시)
		APS_FighterPlayerState* FighterPS = Cast<APS_FighterPlayerState>(PC->PlayerState);
		FString CharacterID = FighterPS ? FighterPS->GetPlayerName() : TEXT("DefaultChar"); // 임시 ID

		// 여기서 실제 스폰은 블루프린트나 Spawner로 대체 예정
		UE_LOG(LogTemp, Log, TEXT("Spawning character for %s (ID: %s)"), *PC->GetName(), *CharacterID);
		// TODO: SpawnActor<CharacterClass>(...) 등으로 캐릭터 생성
	}
}
