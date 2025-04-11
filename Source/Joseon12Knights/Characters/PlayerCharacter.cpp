#include "PlayerCharacter.h"
#include "EnhancedInputComponent.h"
#include "MainPlayerController.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


// Sets default values
APlayerCharacter::APlayerCharacter() : SkillAttackMontage(nullptr), GuardMontage(nullptr), UltimateMontage(nullptr)
{
	PrimaryActorTick.bCanEverTick = true;

	MoveSpeed = 50.f;
	DashDistance = 2000.f;
	JumpMaxCount = 2;
	
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshAsset(TEXT("/Game/ModularAnimalKnightsPolyart/Meshes/OneMeshCharacter/RabitSK.RabitSK"));
	///Game/ModularAnimalKnightsPolyart/Meshes/OneMeshCharacter/RabitSK.RabitSK

	if (MeshAsset.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(MeshAsset.Object);
	}

	GetMesh()->SetRelativeLocation(FVector(0, 0, -90.f));
	FRotator CurrentRotation = FRotator(0.f, -90.f, 0.f);
	GetMesh()->SetWorldRotation(CurrentRotation);

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimBPClass(TEXT("/Game/Blueprints/Animations/ABP_Character.ABP_Character_C"));
	if (AnimBPClass.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(AnimBPClass.Class);
	}

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 300.0f;
	SpringArm->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	NormalAttackMontageIndex = 0;

	WeaponComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon"));
	WeaponComponent->SetupAttachment(GetMesh(), FName("WeaponSocket"));

	WeaponComponent->SetRelativeRotation(FRotator(0.f, 0.f, -180.f));

	ShieldComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Shield"));
	ShieldComponent->SetupAttachment(GetMesh(), FName("ShieldSocket"));

	BuffComponent = CreateDefaultSubobject<UBuffComponent>(TEXT("BuffComponent"));
	StatComponent = CreateDefaultSubobject<UStatComponent>(TEXT("StatComponent"));
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	//WeaponComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, FName(TEXT("WeaponSocket")));
	//ShieldComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, FName(TEXT("ShieldSocket")));
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (AMainPlayerController* PlayerController = Cast<AMainPlayerController>(GetController()))
		{
			if (PlayerController->MoveAction)
			{
				EnhancedInput->BindAction(
					PlayerController->MoveAction,
					ETriggerEvent::Triggered,
					this,
					&APlayerCharacter::Move
				);
			}

			if (PlayerController->JumpAction)
			{
				EnhancedInput->BindAction(
					PlayerController->JumpAction,
					ETriggerEvent::Started,
					this,
					&APlayerCharacter::StartJump
				);
			}

			if (PlayerController->NormalAttackAction)
			{
				EnhancedInput->BindAction(
					PlayerController->NormalAttackAction,
					ETriggerEvent::Started,
					this,
					&APlayerCharacter::NormalAttack
				);
			}

			if (PlayerController->RollAction)
			{
				EnhancedInput->BindAction(
					PlayerController->RollAction,
					ETriggerEvent::Started,
					this,
					&APlayerCharacter::Roll
				);
			}

			if (PlayerController->SkillAction)
			{
				EnhancedInput->BindAction(
					PlayerController->SkillAction,
					ETriggerEvent::Started,
					this,
					&APlayerCharacter::Skill
				);
			}

			if (PlayerController->GuardAction)
			{
				EnhancedInput->BindAction(
					PlayerController->GuardAction,
					ETriggerEvent::Triggered,
					this,
					&APlayerCharacter::Guard
				);
			}

			if (PlayerController->GuardAction)
			{
				EnhancedInput->BindAction(
					PlayerController->GuardAction,
					ETriggerEvent::Completed,
					this,
					&APlayerCharacter::ReleaseGuard
				);
			}

			if (PlayerController->UltimateAction)
			{
				EnhancedInput->BindAction(
					PlayerController->UltimateAction,
					ETriggerEvent::Started,
					this,
					&APlayerCharacter::Ultimate
				);
			}
		}
	}
}

void APlayerCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
	UE_LOG(LogTemp, Warning, TEXT("Land"));
	bIsDoubleJump = false;
}

void APlayerCharacter::Move(const FInputActionValue& Value)
{
	float AxisValue = Value.Get<float>();

	AddMovementInput(GetActorForwardVector(), AxisValue * MoveSpeed);

	float Direction = AxisValue < 0 ? 1 : -1;
	FRotator CurrentRotation = FRotator(0.f, Direction * 90.f, 0.f);
	GetMesh()->SetWorldRotation(CurrentRotation);
}

void APlayerCharacter::StartJump(const FInputActionValue& Value)
{
	if (JumpCurrentCount == 1)
	{
		bIsDoubleJump = true;
		UE_LOG(LogTemp, Warning, TEXT("Double Jump"));
	}

	Jump();

	// ���� ���� ���
	if (JumpSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, JumpSound, GetActorLocation());
	}

	UE_LOG(LogTemp, Warning, TEXT("Jump %d"), JumpCurrentCount);
}

void APlayerCharacter::StopJump(const FInputActionValue& Value)
{
	if (!Value.Get<bool>())
	{
		//StopJumping();
	}
}

void APlayerCharacter::Roll(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("ROLL"));

	if (UCharacterMovementComponent* MoveComp = Cast<UCharacterMovementComponent>(GetMovementComponent()))
	{
		if (!MoveComp->IsFalling())
		{
			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

			if (AnimInstance && DashMontage && !AnimInstance->Montage_IsPlaying(DashMontage))
			{
				AnimInstance->StopAllMontages(1);
				AnimInstance->Montage_Play(DashMontage);
			}

			FVector Direction = GetMesh()->GetRightVector() * DashDistance;
			LaunchCharacter(Direction, true, false);
		}
	}
}

void APlayerCharacter::Guard(const FInputActionValue& Value)
{
		bool bIsGuard = Value.Get<bool>();

		UE_LOG(LogTemp, Warning, TEXT("GUARD %d "), bIsGuard);

		// ���� ���
		if (!bIsGuarding && bIsGuard && GuardSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, GuardSound, GetActorLocation());
		}

		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

		if (AnimInstance && GuardMontage && !AnimInstance->Montage_IsPlaying(GuardMontage))
		{
			AnimInstance->StopAllMontages(1);
			AnimInstance->Montage_Play(GuardMontage);
		}

		bIsGuarding = true;
}

void APlayerCharacter::ReleaseGuard(const FInputActionValue& Value)
{
	bool bIsGuard = Value.Get<bool>();
	bIsGuarding = false;

	UE_LOG(LogTemp, Warning, TEXT("Release GUARD %d "), bIsGuard);

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance && GuardMontage && !AnimInstance->Montage_IsPlaying(GuardMontage))
	{
		AnimInstance->Montage_Stop(0.1f, GuardMontage);
	}
}

float APlayerCharacter::CalculateDamage(float BaseDamage, APlayerCharacter* Attacker)
{
	float AttackerMultiplier = 1.0f;
	if (Attacker)
	{
		if (UBuffComponent* AttackerBuff = Attacker->FindComponentByClass<UBuffComponent>())
		{
			AttackerMultiplier = AttackerBuff->GetAttackMultiplier();
		}
	}

	float DefenderMultiplier = 1.0f;
	if (UBuffComponent* DefenderBuff = FindComponentByClass<UBuffComponent>())
	{
		DefenderMultiplier = DefenderBuff->GetDefenseMultiplier();
	}

	// ����: ���� ������ ���ϰ�, ��� ������ ������ ���� �������� ���
	return BaseDamage * AttackerMultiplier / DefenderMultiplier;
}

// ======================
// RatKnight Targeting Logic
// ======================
APlayerCharacter* APlayerCharacter::GetTargetPlayer()
{
	FVector AttackOrigin; // ���� ���� ����

	// ���Ⱑ ������ ���� ��ġ�� �������� ����
	if (WeaponComponent && WeaponComponent->DoesSocketExist(FName("WeaponSocket")))
	{
		AttackOrigin = WeaponComponent->GetSocketLocation(FName("WeaponSocket"));
	}
	// ������ ĳ���� ���濡 ������ ����
	else
	{
		AttackOrigin = GetActorLocation() + GetActorForwardVector() * ForwardOffset;
	}
	
	TArray<APlayerCharacter*> PotentialTargets = FindTargetsInRadius(AttackOrigin, AttackRadius); // ���� ���� ���� ��� ĳ���͸� ã��
	APlayerCharacter* BestTarget = SelectBestTarget(PotentialTargets); // ���� ������ Ÿ�� ����

	if (bDebugTargeting) // ����� �ð�ȭ
	{
		DrawDebugSphere(GetWorld(), AttackOrigin, AttackRadius, 24,
			BestTarget ? FColor::Green : FColor::Red, false, 1.0f);

		if (BestTarget)
		{
			DrawDebugLine(GetWorld(), AttackOrigin, BestTarget->GetActorLocation(),
				FColor::Yellow, false, 1.0f, 0, 2.0f);
		}
	}

	return BestTarget;
}

TArray<APlayerCharacter*> APlayerCharacter::FindTargetsInRadius(const FVector& Origin, float Radius)
{
	TArray<APlayerCharacter*> FoundTargets;
	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this); // �ڽ��� ����

	// ��ü ������ �˻� ����
	bool bOverlapFound = GetWorld()->OverlapMultiByObjectType(
		OverlapResults,
		Origin,
		FQuat::Identity,
		FCollisionObjectQueryParams(TargetCollisionChannel), // ������ ä�ο� ���� ����
		FCollisionShape::MakeSphere(Radius),
		QueryParams
	);

	if (bOverlapFound)
	{
		// ������� APlayerCharacter Ÿ�Ը� ���͸�
		for (const FOverlapResult& Overlap : OverlapResults)
		{
			APlayerCharacter* PlayerChar = Cast<APlayerCharacter>(Overlap.GetActor());
			if (PlayerChar && PlayerChar != this)
			{
				// �� ��� �����̶�� �� ���� Ÿ�����ϵ��� ���͸� ����
				// if (PlayerChar->GetTeam() != GetTeam()) 
				// {
				FoundTargets.Add(PlayerChar);
				// }
			}
		}
	}

	return FoundTargets;
}

APlayerCharacter* APlayerCharacter::SelectBestTarget(const TArray<APlayerCharacter*>& PotentialTargets)
{
	if (PotentialTargets.Num() == 0)
	{
		return nullptr;
	}

	// Ÿ�� ���� ����:
	// 1. �켱����: ���� �þ߰� ���� �ִ� ��
	// 2. ����å: ���� ����� ��

	APlayerCharacter* BestTarget = nullptr;
	float BestScore = -1.0f;

	FVector Forward = GetActorForwardVector();
	FVector ActorLocation = GetActorLocation();

	const float ViewAngleCos = FMath::Cos(FMath::DegreesToRadians(60.0f)); // 60�� �þ߰�
	const float ViewAngleWeight = 2.0f; // �þ߰� �� ��� ����ġ

	for (APlayerCharacter* Target : PotentialTargets)
	{
		if (!Target){continue;}

		FVector ToTarget = Target->GetActorLocation() - ActorLocation;
		float Distance = ToTarget.Size();
		
		float DistanceScore = 1.0f / FMath::Max(Distance, 1.0f); // �Ÿ��� �ּ��� ���� ���� (���� ���)
		
		ToTarget.Normalize(); // ���� ����ȭ

		float DotProduct = FVector::DotProduct(Forward, ToTarget); // ���� ���Ϳ� Ÿ�� ���� ������ ���� ���

		float AngleScore = DotProduct > ViewAngleCos ? ViewAngleWeight : 1.0f; // �þ߰� ���� �ִ��� Ȯ�� (DotProduct > ViewAngleCos�� �þ߰� ���� ����)

		float FinalScore = DistanceScore * AngleScore; // ���� ���� ���

		// �� ���� ������ Ÿ���̸� ����
		if (FinalScore > BestScore)
		{
			BestScore = FinalScore;
			BestTarget = Target;
		}
	}
	return BestTarget;
}