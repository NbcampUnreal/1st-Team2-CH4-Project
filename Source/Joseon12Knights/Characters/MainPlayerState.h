

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "MainPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class JOSEON12KNIGHTS_API AMainPlayerState : public APlayerState
{
	GENERATED_BODY()
	

private:
	
protected:
	UPROPERTY(BlueprintReadOnly, Category = "Status")
	float MaxHealth;
	UPROPERTY(BlueprintReadOnly, Category = "Status")
	float CurrentHealth;

	UPROPERTY(BlueprintReadOnly, Category = "Status")
	int32 Stock;
public:
	AMainPlayerState();

	UFUNCTION(BlueprintCallable)
	void Initialize();

	float GetPlayerHealth() const;
	void SetPlayerStatus(float Health);
	void SetDamage(float Damage);

	UFUNCTION(BlueprintCallable, Category = "Status")
	int32 GetStock() const { return Stock; }

	void SetStock(int32 NewStock) { Stock = NewStock; }
};
