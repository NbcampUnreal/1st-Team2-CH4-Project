


#include "AttackAnimNotify.h"
#include "GameFramework/Actor.h"
#include "Characters/PlayerCharacter.h"


void UAttackAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    if (APlayerCharacter* Character = Cast<APlayerCharacter>(MeshComp->GetOwner()))
    {
        //Character->HandleAttackHit();  // 캐릭터에 함수 만들어서 호출
        //UE_LOG(LogTemp, Warning, TEXT("Hit"));
    }
}

