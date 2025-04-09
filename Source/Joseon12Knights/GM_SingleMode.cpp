// GM_SingleMode.cpp
#include "GM_SingleMode.h"
#include "Kismet/GameplayStatics.h"

void AGM_SingleMode::BeginPlay()
{
	Super::BeginPlay();

	StartCharacterSelect();
}

void AGM_SingleMode::ProceedToMatch()
{
	UE_LOG(LogTemp, Log, TEXT("SingleMode: Proceeding to match (OpenLevel ConflictZone)"));
	UGameplayStatics::OpenLevel(this, FName("ConflictZone"));
}
