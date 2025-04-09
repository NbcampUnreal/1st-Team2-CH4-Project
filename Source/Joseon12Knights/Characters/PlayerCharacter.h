

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterComponent/BuffComponent.h"
#include "CharacterComponent/StatComponent.h"
#include "PlayerCharacter.generated.h"

struct FInputActionValue;
class UCameraComponent;
class USpringArmComponent;

UCLASS(Abstract, NotBlueprintable)
class JOSEON12KNIGHTS_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Stat")
	bool bIsDoubleJump;
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float MoveSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float DashDistance; // 1000.f ´ç 1À¯´Ö

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	TArray<UAnimMontage*> NormalAttackMontages;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* SkillAttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* GuardMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* UltimateMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* DashMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	UCameraComponent* Camera;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment")
	UStaticMeshComponent* WeaponComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment")
	UStaticMeshComponent* ShieldComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	UBuffComponent* BuffComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	UStatComponent* StatComponent;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void Landed(const FHitResult& Hit);

	UFUNCTION()
	void Move(const FInputActionValue& Value);

	UFUNCTION()
	void StartJump(const FInputActionValue& Value);

	UFUNCTION()
	void StopJump(const FInputActionValue& Value);

	UFUNCTION()
	void Roll(const FInputActionValue& Value);

	UFUNCTION()
	void Guard(const FInputActionValue& Value);

	UFUNCTION()
	void ReleaseGuard(const FInputActionValue& Value);

	UFUNCTION()
	void NormalAttack(const FInputActionValue& Value)
	{
		int Size = NormalAttackMontages.Num();
		int PrevIndex = NormalAttackMontageIndex;
		NormalAttackMontageIndex++;
		UAnimMontage* NormalAttackMontage = NormalAttackMontages[NormalAttackMontageIndex % Size];

		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

		if (AnimInstance && NormalAttackMontage && !AnimInstance->Montage_IsPlaying(NormalAttackMontages[PrevIndex % Size]))
		{
			AnimInstance->StopAllMontages(1);
			AnimInstance->Montage_Play(NormalAttackMontage);
		}
	}

	UFUNCTION(BlueprintCallable, Category = "Combat")
	float CalculateDamage(float BaseDamage, APlayerCharacter* Attacker);


	UFUNCTION()
	virtual void Skill(const FInputActionValue& Value) PURE_VIRTUAL(APlayerCharacter::Skill, );
	UFUNCTION()
	virtual void Ultimate(const FInputActionValue& Value) PURE_VIRTUAL(APlayerCharacter::Ultimate, );
private:
	int NormalAttackMontageIndex;
};