


#include "BTComposite_RandomSelector.h"

UBTComposite_RandomSelector::UBTComposite_RandomSelector(const FObjectInitializer& ObjectInitializer)
{
	NodeName = "RandomSelector";
}

int32 UBTComposite_RandomSelector::GetNextChildHandler(FBehaviorTreeSearchData& SearchData, int32 PrevChild, EBTNodeResult::Type LastResult) const
{
	int32 NextChildIdx = BTSpecialChild::ReturnToParent;

	if (PrevChild == BTSpecialChild::NotInitialized)
	{
		NextChildIdx = FMath::RandRange(0, GetChildrenNum() - 1);
	}
	else if (LastResult == EBTNodeResult::Succeeded)
	{
		NextChildIdx = FMath::RandRange(0, GetChildrenNum() - 1);
	}

	return NextChildIdx;
}
