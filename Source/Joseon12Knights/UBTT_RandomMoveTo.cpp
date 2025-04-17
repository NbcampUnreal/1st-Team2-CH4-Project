


#include "UBTT_RandomMoveTo.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BlackboardComponent.h"

UUBTT_RandomMoveTo::UUBTT_RandomMoveTo()
{
	NodeName = "Random Move To";
}

EBTNodeResult::Type UUBTT_RandomMoveTo::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AICon = OwnerComp.GetAIOwner();
	if (!AICon)
		return EBTNodeResult::Failed;

	APawn* Pawn = AICon->GetPawn();
	if (!Pawn)
		return EBTNodeResult::Failed;

	UNavigationSystemV1* NavSys = UNavigationSystemV1::GetNavigationSystem(Pawn->GetWorld());
	if (!NavSys)
		return EBTNodeResult::Failed;

	// �̵� ���� ����
	int32 DirectionSign = FMath::RandBool() ? 1 : -1;
	float MoveDistance = FMath::FRandRange(MinDistance, MaxDistance);
	FVector MoveDirection = FVector(1, 0, 0); // �Ǵ� Y�� ����̸� (0,1,0)

	FVector StartLocation = Pawn->GetActorLocation();
	FVector TargetLocation = StartLocation + (MoveDirection * MoveDistance * DirectionSign);

	// ȸ�� ���� ����
	FRotator NewRotation = FRotator::ZeroRotator;
	NewRotation.Yaw = (DirectionSign > 0) ? 0.f : 180.f;
	Pawn->SetActorRotation(NewRotation);

	FNavLocation ResultLocation;
	if (NavSys->ProjectPointToNavigation(TargetLocation, ResultLocation, FVector(100.f)))
	{
		AICon->MoveToLocation(ResultLocation.Location);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
