

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterComponent/BuffComponent.h"
#include "CharacterComponent/StatComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerCharacter.generated.h"

struct FInputActionValue;
class UCameraComponent;
class USpringArmComponent;

UCLASS(Abstract, NotBlueprintable)
class JOSEON12KNIGHTS_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APlayerCharacter();

protected:

	UFUNCTION()
	void OnCapsuleOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION()
	void OnWeaponOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	virtual float TakeDamage(
		float DamageAmount,
		struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator,
		AActor* DamageCauser
	) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Replicated)
	bool bIsDoubleJump;
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsHit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float MoveSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float DashDistance; // 1000.f ´ç 1À¯´Ö

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	TArray<UAnimMontage*> NormalAttackMontages;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* SkillAttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* UltimateMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment")
	UStaticMeshComponent* WeaponComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment")
	UStaticMeshComponent* ShieldComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundBase* NormalAttackSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundBase* GuardSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundBase* JumpSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	UBuffComponent* BuffComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	UStatComponent* StatComponent;
	
	FRotator LastSentRotation;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation")
	TMap<FString, UAnimMontage*> MapAnim;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void Landed(const FHitResult& Hit);

	UFUNCTION()
	void Move(const FInputActionValue& Value);

	UFUNCTION(Server, Unreliable)
	void ServerSetDirection(const FRotator& Rotation);

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastSetDirection(const FRotator& Rotation);

	UFUNCTION()
	void StartJump(const FInputActionValue& Value);

	UFUNCTION(Server, Reliable)
	void ServerStartJump();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastStartJump();

	UFUNCTION()
	void StopJump(const FInputActionValue& Value);

	UFUNCTION()
	void Dash(const FInputActionValue& Value);
	UFUNCTION(Server, Unreliable)
	void ServerDash();
	UFUNCTION(NetMulticast, Reliable)
	void MulticastDash();

	UFUNCTION()
	void Guard(const FInputActionValue& Value);

	UFUNCTION()
	void ReleaseGuard(const FInputActionValue& Value);

	UFUNCTION()
	void NormalAttack(const FInputActionValue& Value);

	UFUNCTION(Server, Unreliable)
	void ServerAttack();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastAttack();

	UFUNCTION(BlueprintCallable, Category = "Combat")
	float CalculateDamage(float BaseDamage, APlayerCharacter* Attacker);

	UFUNCTION()
	virtual void Skill(const FInputActionValue& Value) PURE_VIRTUAL(APlayerCharacter::Skill, );
	UFUNCTION()
	virtual void Ultimate(const FInputActionValue& Value) PURE_VIRTUAL(APlayerCharacter::Ultimate, );
protected:
	bool bIsGuarding;

private:
	int NormalAttackMontageIndex;
};