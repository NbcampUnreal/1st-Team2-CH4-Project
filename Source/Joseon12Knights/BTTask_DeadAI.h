

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_DeadAI.generated.h"

/**
 * 
 */
UCLASS()
class JOSEON12KNIGHTS_API UBTTask_DeadAI : public UBTTaskNode
{
	GENERATED_BODY()
	
	UBTTask_DeadAI();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
