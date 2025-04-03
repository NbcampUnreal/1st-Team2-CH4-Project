

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

struct FInputActionValue;

UCLASS(Abstract, NotBlueprintable)
class JOSEON12KNIGHTS_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float MoveSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float DashDistance; // 1000.f �� 1����

	UFUNCTION()
	void Move(const FInputActionValue& Value);

	UFUNCTION()
	void StartJump(const FInputActionValue& Value);

	UFUNCTION()
	void StopJump(const FInputActionValue& Value);

	UFUNCTION()
	void Roll(const FInputActionValue& Value);

	UFUNCTION()
	virtual void NormalAttack(const FInputActionValue& Value) PURE_VIRTUAL(APlayerCharacter::NormalAttack, );
	UFUNCTION()
	virtual void Skill(const FInputActionValue& Value) PURE_VIRTUAL(APlayerCharacter::Skill, );
	UFUNCTION()
	virtual void Ultimate(const FInputActionValue& Value) PURE_VIRTUAL(APlayerCharacter::Ultimate, );


};