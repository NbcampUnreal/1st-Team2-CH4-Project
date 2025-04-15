

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AttackAnimNotify.generated.h"

/**
 * 
 */
UCLASS()
class JOSEON12KNIGHTS_API UAttackAnimNotify : public UAnimNotify
{
	GENERATED_BODY()
	
	
public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;	
};
