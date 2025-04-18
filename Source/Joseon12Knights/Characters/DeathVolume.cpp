


#include "DeathVolume.h"
#include "Components/BoxComponent.h"
#include "PlayerCharacter.h"
#include "CharacterComponent/StatComponent.h"
#include "Kismet/GameplayStatics.h"

ADeathVolume::ADeathVolume()
{
	PrimaryActorTick.bCanEverTick = false;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	RootComponent = BoxComponent;

	BoxComponent->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));
#if WITH_EDITOR 
	BoxComponent->bHiddenInGame = false;
#endif
}

void ADeathVolume::BeginPlay()
{
	Super::BeginPlay();
	// 오버랩 델리게이트 바인딩
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ADeathVolume::HandleOverlap);
}

void ADeathVolume::HandleOverlap(
    UPrimitiveComponent*,
    AActor* OtherActor,
    UPrimitiveComponent*,
    int32,
    bool,
    const FHitResult&)
{
    // 멀티플레이 : 서버에서만 처리
    if (!HasAuthority()) return;

    if (APlayerCharacter* PC = Cast<APlayerCharacter>(OtherActor))
    {
        // StatComponent로 HP = 0 만들기
        if (UStatComponent* Stat = PC->GetStatComponent())
        {
            const float Current = Stat->GetCurrentHP();
            if (Current > 0.f)
            {
                Stat->ChangeHP(-Current);   //  0이 되면 OnDeath 브로드캐스트
            }
        }

        // 9999 대미지를 넣어 TakeDamage → Dead() 호출
         UGameplayStatics::ApplyDamage(PC, 9999.f, nullptr, this, nullptr);
    }
}