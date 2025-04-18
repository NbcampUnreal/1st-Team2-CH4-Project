

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DeathVolume.generated.h"

UCLASS()
class JOSEON12KNIGHTS_API ADeathVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	ADeathVolume();

protected:
	// 장외 영역 Box Collision
	UPROPERTY(VisibleAnywhere, Category = "Collision")
	class UBoxComponent* BoxComponent;

	virtual void BeginPlay() override;

	UFUNCTION()
	void HandleOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32               OtherBodyIndex,
		bool                bFromSweep,
		const FHitResult& SweepResult);
};
