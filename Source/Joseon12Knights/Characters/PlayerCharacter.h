

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterComponent/BuffComponent.h"
#include "CharacterComponent/StatComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerCharacter.generated.h"

struct FInputActionValue;
class UCameraComponent;
class USpringArmComponent;
class UWidgetComponent;

UCLASS(Abstract, NotBlueprintable)
class JOSEON12KNIGHTS_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APlayerCharacter();

	UFUNCTION(BlueprintCallable)
	void Respawn();

	UFUNCTION(BlueprintCallable)
	void InitializeData();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	UWidgetComponent* OverheadWidget;

protected:
	bool bIsAlive;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float AttackDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float CurrentHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float MaxHealth;
	float Defense;

#pragma region Guard
	UFUNCTION()
	void UpdateGauge(float FillAmount);

	UFUNCTION()
	void RechargeGuardGauge();

	UFUNCTION()
	void GuardTimer();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Temp")
	float RemainTime;

	const float GuardDuration = 5.0f;
	float RechargeGuard;
	bool bIsMaxGuardGauge;
	bool bCanGuard;
	FTimerHandle GuardTimerHandle;
	FTimerHandle ReleaseGuardTimerHandle;
#pragma endregion

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
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float MoveSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float DashDistance; // 1000.f �� 1����

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



	UFUNCTION(BlueprintCallable)
	bool IsAlive() const;

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
	UFUNCTION(Server, Reliable)
	void ServerDash();
	UFUNCTION(NetMulticast, Reliable)
	void MulticastDash();

	UFUNCTION()
	void Guard(const FInputActionValue& Value);

	UFUNCTION(Server, Unreliable)
	void ServerGuard();

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastGuard();

	UFUNCTION()
	void ReleaseGuard(const FInputActionValue& Value);
	UFUNCTION(Server, Reliable)
	void ServerReleaseGuard();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastReleaseGuard();

	UFUNCTION()
	void NormalAttack(const FInputActionValue& Value);

	UFUNCTION(Server, Reliable)
	void ServerAttack();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastAttack();

	UFUNCTION()
	void Dead();

	UFUNCTION(Server, Reliable)
	void ServerDead();
	UFUNCTION(NetMulticast, Reliable)
	void MulticastDead();

	UFUNCTION()
	void BeginAttack();

	UFUNCTION()
	void EndAttack();

	UFUNCTION(BlueprintCallable, Category = "Combat")
	float CalculateDamage(float BaseDamage, APlayerCharacter* Attacker);

	UFUNCTION()
	virtual void Skill(const FInputActionValue& Value);

	UFUNCTION(Server, Reliable)
	virtual void ServerSkill();

	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastSkill();

	UFUNCTION()
	virtual void Ultimate(const FInputActionValue& Value);

	UFUNCTION(Server, Reliable)
	virtual void ServerUltimate();

	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastUltimate();

protected:
	bool bIsGuarding;
	bool bIsHit;
	bool bIsDead;
private:
	int NormalAttackMontageIndex;
public:
	UFUNCTION()
	void SetHitState(bool bIsHit);
	// ==============
	// Targeting Logic
	// ==============
	UPROPERTY(EditAnywhere, Category = "Targeting")
	float AttackRadius = 106.0f;  // ���� ���� �ݰ�

	UPROPERTY(EditAnywhere, Category = "Targeting")
	float ForwardOffset = 30.0f; // ���� ������ �Ÿ�

	UPROPERTY(EditAnywhere, Category = "Targeting")
	bool bDebugTargeting = false; // ����� �ð�ȭ ����

	UPROPERTY(EditAnywhere, Category = "Targeting")
	TEnumAsByte<ECollisionChannel> TargetCollisionChannel = ECC_Pawn; // Ÿ�� ���� ä��

	// ���� ��� ã�� �Լ�
	APlayerCharacter* GetTargetPlayer();
	TArray<APlayerCharacter*> FindTargetsInRadius(const FVector& Origin, float Radius);
	APlayerCharacter* SelectBestTarget(const TArray<APlayerCharacter*>& PotentialTargets);

public:
	UFUNCTION(BlueprintCallable, Category = "Component")
	UStatComponent* GetStatComponent() const { return StatComponent; }

protected:
	// �̵��ӵ� ������Ʈ
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void UpdateMovementSpeed();

	UFUNCTION()
	void Test(UAnimMontage* Montage, bool bInterrupted);

};