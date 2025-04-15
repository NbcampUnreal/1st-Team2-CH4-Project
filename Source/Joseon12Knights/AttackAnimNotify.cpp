


#include "AttackAnimNotify.h"
#include "GameFramework/Actor.h"
#include "Characters/PlayerCharacter.h"


void UAttackAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    if (APlayerCharacter* Character = Cast<APlayerCharacter>(MeshComp->GetOwner()))
    {
        
    }
}

