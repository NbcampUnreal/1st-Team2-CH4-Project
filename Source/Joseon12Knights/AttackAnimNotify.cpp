


#include "AttackAnimNotify.h"
#include "GameFramework/Actor.h"
#include "Characters/PlayerCharacter.h"


void UAttackAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    if (APlayerCharacter* Character = Cast<APlayerCharacter>(MeshComp->GetOwner()))
    {
        //Character->HandleAttackHit();  // ĳ���Ϳ� �Լ� ���� ȣ��
        //UE_LOG(LogTemp, Warning, TEXT("Hit"));
    }
}

