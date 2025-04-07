// GM_SingleMode.cpp
#include "GM_SingleMode.h"
#include "Kismet/GameplayStatics.h"

void AGM_SingleMode::BeginPlay()
{
	Super::BeginPlay();

	// 싱글 모드는 즉시 캐릭터 셀렉 시작
	StartCharacterSelect();
}

void AGM_SingleMode::ProceedToMatch()
{
	UE_LOG(LogTemp, Log, TEXT("SingleMode: Proceeding to match (OpenLevel ConflictZone)"));
	UGameplayStatics::OpenLevel(this, FName("ConflictZone"));
}
