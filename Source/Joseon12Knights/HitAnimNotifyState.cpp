#include "HitAnimNotifyState.h"
#include "GameFramework/Actor.h"
#include "Characters/PlayerCharacter.h"

void UHitAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(MeshComp->GetOwner()))
	{
		PlayerCharacter->SetHitState(true);
	}
}

void UHitAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(MeshComp->GetOwner()))
	{
		PlayerCharacter->SetHitState(false);
	}
}
