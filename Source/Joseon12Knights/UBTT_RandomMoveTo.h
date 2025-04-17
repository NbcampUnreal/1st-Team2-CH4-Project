#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "UBTT_RandomMoveTo.generated.h"

/**
 * 
 */
UCLASS()
class JOSEON12KNIGHTS_API UUBTT_RandomMoveTo : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UUBTT_RandomMoveTo();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere, Category = "AI")
	float MinDistance = 300.f;

	UPROPERTY(EditAnywhere, Category = "AI")
	float MaxDistance = 600.f;
};
