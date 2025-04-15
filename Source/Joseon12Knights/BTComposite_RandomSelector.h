

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTCompositeNode.h"
#include "BTComposite_RandomSelector.generated.h"

/**
 * 
 */
UCLASS()
class JOSEON12KNIGHTS_API UBTComposite_RandomSelector : public UBTCompositeNode
{
	GENERATED_BODY()
	
    explicit UBTComposite_RandomSelector(const FObjectInitializer& ObjectInitializer);

	virtual int32 GetNextChildHandler(struct FBehaviorTreeSearchData& SearchData, int32 PrevChild, EBTNodeResult::Type LastResult) const override;
};
