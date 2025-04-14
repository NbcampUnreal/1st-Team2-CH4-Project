


#include "AIC_Enemy.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

AAIC_Enemy::AAIC_Enemy()
{

}

void AAIC_Enemy::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AAIC_Enemy::RandomMove, 1.5f, true);
}

void AAIC_Enemy::OnUnPossess()
{
	Super::OnUnPossess();


	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
}

void AAIC_Enemy::RandomMove()
{
	auto* ControlledPawn = GetPawn();
	if (!ControlledPawn)
		return;

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	if (!NavSystem)
		return;

	FVector StartLocation = ControlledPawn->GetActorLocation();

	// 무작위로 왼쪽(-1) 또는 오른쪽(+1)
	int32 DirectionSign = FMath::RandBool() ? 1 : -1;

	// 이동 거리: 300~600 사이에서 랜덤
	float MoveDistance = FMath::FRandRange(300.f, 600.f);

	// 월드 기준 X축 방향으로 이동 (1,0,0)
	FVector MoveDirection = FVector(1, 0, 0); // 또는 FVector(0, 1, 0) → Y축 기반 게임이면

	FVector TargetLocation = StartLocation + (MoveDirection * MoveDistance * DirectionSign);

	FNavLocation NavLocation;
	if (NavSystem->ProjectPointToNavigation(TargetLocation, NavLocation, FVector(100.f, 100.f, 100.f)))
	{
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, NavLocation);
	}
}