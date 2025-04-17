#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "MainPlayerState.generated.h"

UCLASS()
class JOSEON12KNIGHTS_API AMainPlayerState : public APlayerState
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Status")
	float MaxHealth;

	UPROPERTY(BlueprintReadOnly, Category = "Status")
	float CurrentHealth;

	UPROPERTY(ReplicatedUsing = OnRep_Stock, BlueprintReadOnly, Category = "Status")
	int32 Stock;

public:
	AMainPlayerState();

	UFUNCTION(BlueprintCallable)
	void Initialize();

	float GetMaxHealth() const { return MaxHealth; }
	float GetCurrentHealth() const { return CurrentHealth; }
	float GetPlayerHealth() const;
	void SetPlayerStatus(float Health);
	void SetDamage(float Damage);

	UFUNCTION(BlueprintCallable, Category = "Status")
	int32 GetStock() const { return Stock; }

	void SetStock(int32 NewStock) { Stock = NewStock; }

	UFUNCTION()
	void OnRep_Stock();

	UPROPERTY(BlueprintReadOnly)
	int32 PlayerIndex = -1;

	int32 GetPlayerIndex() const { return PlayerIndex; }
	void SetPlayerIndex(int32 Index) { PlayerIndex = Index; }



	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
