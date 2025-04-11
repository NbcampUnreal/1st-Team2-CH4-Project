#include "PlayerCharacter.h"
#include "EnhancedInputComponent.h"
#include "MainPlayerController.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
APlayerCharacter::APlayerCharacter() : SkillAttackMontage(nullptr), GuardMontage(nullptr), UltimateMontage(nullptr), DashMontage(nullptr)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
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

	WeaponComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	ShieldComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

	UCapsuleComponent* Capsule = GetCapsuleComponent();
	
	Capsule->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Capsule->SetCollisionObjectType(ECC_Pawn); 
	Capsule->SetCollisionResponseToAllChannels(ECR_Ignore);
	Capsule->SetCollisionResponseToChannel(ECC_Visibility, ECR_Overlap);
	Capsule->SetCollisionResponseToChannel(ECC_Camera, ECR_Block);
	Capsule->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
	Capsule->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	Capsule->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Overlap);
	Capsule->CanCharacterStepUpOn = ECB_No;

	Capsule->ComponentTags.Add("Player");

	BuffComponent = CreateDefaultSubobject<UBuffComponent>(TEXT("BuffComponent"));
	StatComponent = CreateDefaultSubobject<UStatComponent>(TEXT("StatComponent"));

}

void APlayerCharacter::Test()
{
	UE_LOG(LogTemp, Warning, TEXT("TEST : %s"), *GetName());
}

void APlayerCharacter::OnCapsuleOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//if (OtherActor == this) return;

	//if (OtherComp && OtherComp->ComponentHasTag("Player"))
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Overlap Weapon"));
	//}
}

void APlayerCharacter::OnWeaponOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//if (OtherActor == this) return;

	//if (OtherComp && OtherComp->ComponentHasTag("Player"))
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Overlap Weapon"));
	//}
	

	if (OtherActor == this) return;

	if (APlayerCharacter* EnemyCharacter = Cast<APlayerCharacter>(OtherActor))
	{
		UE_LOG(LogTemp, Warning, TEXT("Overlap Weapon"));
		
		UGameplayStatics::ApplyDamage(EnemyCharacter, 50.f, GetController(), this, UDamageType::StaticClass());
	}
}

float APlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	UE_LOG(LogTemp, Warning, TEXT("Take Damage"));
	float KnockBackDistance = bIsGuarding ? 500.f : 1000.f;

	FVector Direction = DamageCauser->GetActorLocation() - GetActorLocation();

	//FVector Direction = GetMesh()->GetRightVector() * DashDistance;
	LaunchCharacter(-Direction.GetSafeNormal() * KnockBackDistance, true, false);

	return 0.0f;
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnCapsuleOverlap);
	WeaponComponent->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnWeaponOverlap);
}

void APlayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APlayerCharacter, bIsDoubleJump);
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

	float Direction = AxisValue < 0 ? 1 : -1;

	AddMovementInput(GetActorForwardVector(), AxisValue * MoveSpeed);

	FRotator CurrentRotation = FRotator(0.f, Direction * 90.f, 0.f);
	GetMesh()->SetWorldRotation(CurrentRotation);
}

void APlayerCharacter::StartJump(const FInputActionValue& Value)
{
	ServerStartJump();
}

void APlayerCharacter::ServerStartJump_Implementation()
{
	MulticastStartJump();
}

void APlayerCharacter::MulticastStartJump_Implementation()
{
	//JumpCount++;
	if (JumpCurrentCount == 1)
	{
		bIsDoubleJump = true;
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

void APlayerCharacter::NormalAttack(const FInputActionValue& Value)
{
	ServerAttack();
}

void APlayerCharacter::ServerAttack_Implementation()
{
	MulticastAttack();
}

void APlayerCharacter::MulticastAttack_Implementation()
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

	// ���� ���
	if (NormalAttackSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, NormalAttackSound, GetActorLocation());
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