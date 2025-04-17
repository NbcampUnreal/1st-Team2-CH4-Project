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
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "MainPlayerState.h"
#include "Components/WidgetComponent.h"
#include "Components/ProgressBar.h" 


// Sets default values
APlayerCharacter::APlayerCharacter() : SkillAttackMontage(nullptr), UltimateMontage(nullptr)
{
	PrimaryActorTick.bCanEverTick = true;
	bIsAlive = true;
	TMap<FString, FString> MapAnimNamePath;
	MapAnimNamePath.Add("Hit", "/Game/ModularAnimalKnightsPolyart/Animations/AM_GetHitAnim.AM_GetHitAnim");
	MapAnimNamePath.Add("Death", "/Game/ModularAnimalKnightsPolyart/Animations/AM_Die.AM_Die");
	MapAnimNamePath.Add("Guard", "/Game/ModularAnimalKnightsPolyart/Animations/AM_Defend.AM_Defend");
	MapAnimNamePath.Add("GuardHit", "/Game/ModularAnimalKnightsPolyart/Animations/AM_DefendHit.AM_DefendHit");
	MapAnimNamePath.Add("Dash", "/Game/ModularAnimalKnightsPolyart/Animations/AM_DashForward.AM_DashForward");

	for (const TPair<FString, FString>& Entry : MapAnimNamePath)
	{
		FStringAssetReference AssetRef(Entry.Value);
		UAnimMontage* LoadedMontage = LoadObject<UAnimMontage>(nullptr, *Entry.Value);
		if (LoadedMontage)
		{
			MapAnim.Add(Entry.Key, LoadedMontage);
		}
	}
	MoveSpeed = 50.f;
	DashDistance = 2000.f;
	JumpMaxCount = 2;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshAsset(TEXT("/Game/ModularAnimalKnightsPolyart/Meshes/OneMeshCharacter/RabitSK.RabitSK"));

	if (MeshAsset.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(MeshAsset.Object);
	}

	GetMesh()->SetRelativeLocation(FVector(0, 0, -90.f));
	FRotator CurrentRotation = FRotator(0.f, -90.f, 0.f);
	GetMesh()->SetWorldRotation(CurrentRotation);
	GetMesh()->SetIsReplicated(false);

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimBPClass(TEXT("/Game/Blueprints/Animations/ABP_Character.ABP_Character_C"));
	if (AnimBPClass.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(AnimBPClass.Class);
	}

	NormalAttackMontageIndex = 0;

	WeaponComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon"));
	WeaponComponent->SetupAttachment(GetMesh(), FName("WeaponSocket"));

	WeaponComponent->SetRelativeRotation(FRotator(0.f, 0.f, -180.f));

	ShieldComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Shield"));
	ShieldComponent->SetupAttachment(GetMesh(), FName("ShieldSocket"));

	WeaponComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	WeaponComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
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

	GetCharacterMovement()->bOrientRotationToMovement = false;
	bUseControllerRotationYaw = false;

	OverheadWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("OverheadWidget"));
	OverheadWidget->SetupAttachment(GetMesh());
	OverheadWidget->SetWidgetSpace(EWidgetSpace::Screen);
}

void APlayerCharacter::Respawn()
{
	InitializeData();
}

void APlayerCharacter::InitializeData()
{
	MaxHealth = StatComponent->GetMaxHP();
	bIsAlive = true;
	
	AMainPlayerState* PS = GetPlayerState<AMainPlayerState>();
	if (PS)
	{
		PS->SetPlayerStatus(MaxHealth);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Fail"));
	}
}


void APlayerCharacter::UpdateGauge(float FillAmount)
{
	if (!OverheadWidget) return;
	UUserWidget* Gauge = OverheadWidget->GetUserWidgetObject();

	if (!Gauge) return;
	if (UProgressBar* PB = Cast<UProgressBar>(Gauge->GetWidgetFromName(TEXT("ProgressBar_257"))))
	{
		PB->SetPercent(FillAmount);
	}

}

void APlayerCharacter::OnCapsuleOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}

void APlayerCharacter::OnWeaponOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == this) return;

	if (APlayerCharacter* EnemyCharacter = Cast<APlayerCharacter>(OtherActor))
	{
		EndAttack();
		UGameplayStatics::ApplyDamage(EnemyCharacter, AttackDamage, GetController(), this, UDamageType::StaticClass());
	}
}

float APlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	//if (!IsLocallyControlled()) return 0.0f;
	if (bIsHit) return 0.0f;
	
	float KnockBackDistance = bIsGuarding ? 500.f : 1000.f;

	FVector Direction = DamageCauser->GetActorLocation() - GetActorLocation();

	LaunchCharacter(-Direction.GetSafeNormal() * KnockBackDistance, true, false);

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	CurrentHealth -= DamageAmount;
	
	UE_LOG(LogTemp, Warning, TEXT("Hit ! Current Health : %f"), CurrentHealth);

	if (CurrentHealth <= 0)
	{
		CurrentHealth = 0;
		Dead();
	}
	else
	{
		FString AnimKey = bIsGuarding ? TEXT("GuardHit") : TEXT("Hit");
		AMainPlayerState* PS = GetPlayerState<AMainPlayerState>();
		if (PS)
		{
			PS->SetDamage(DamageAmount);
		}

		UAnimMontage* AM = MapAnim[AnimKey];

		if (AnimInstance && AM && !AnimInstance->Montage_IsPlaying(AM))
		{
			AnimInstance->StopAllMontages(1);
			AnimInstance->Montage_Play(AM);
		}
	}

	return DamageAmount;
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnCapsuleOverlap);
	WeaponComponent->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnWeaponOverlap);

	AttackDamage = StatComponent->GetAttack();
	MaxHealth = StatComponent->GetMaxHP();
	CurrentHealth = MaxHealth;
}

void APlayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APlayerCharacter, bIsDoubleJump);
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateMovementSpeed();

	if (GetWorld()->GetTimerManager().IsTimerActive(Timer))
	{
		RemainTime = GetWorld()->GetTimerManager().GetTimerRemaining(Timer);
		UpdateGauge(RemainTime / 2);
		//UE_LOG(LogTemp, Warning, TEXT("%f"), RemainTime / 2);
	}
}

void APlayerCharacter::UpdateMovementSpeed() // Îß??ÑÎ†à?ÑÎßà???∏Ï∂ú?òÏñ¥ ?¥Îèô ?çÎèÑÎ•??ÖÎç∞?¥Ìä∏.
{
	if (GetCharacterMovement() && BuffComponent)
	{
		float EffectiveSpeed = MoveSpeed * BuffComponent->GetMoveSpeedMultiplier();
		GetCharacterMovement()->MaxWalkSpeed = EffectiveSpeed;
	}
}

void APlayerCharacter::Test(UAnimMontage* Montage, bool bInterrupted)
{

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
					&APlayerCharacter::Dash
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

void APlayerCharacter::TestTimer()
{
	if (GetWorld()->GetTimerManager().IsTimerActive(Timer))
	{
		return;
	}
	else
	{
		GetWorldTimerManager().SetTimer(
			Timer,
			FTimerDelegate::CreateLambda([this]()
				{
					UE_LOG(LogTemp, Warning, TEXT("Test Timer!!"));

				}),
			2.0f,
			false
		);
	}


}


bool APlayerCharacter::IsAlive() const
{
	return bIsAlive;
}

void APlayerCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
	bIsDoubleJump = false;
}

void APlayerCharacter::Move(const FInputActionValue& Value)
{
	if (bIsHit || !bIsAlive) return;
	float AxisValue = Value.Get<float>();
	FVector Movement = FVector(AxisValue * MoveSpeed, 0.f, 0.f);
	AddMovementInput(Movement.GetSafeNormal(), 1.f);

	FRotator DesiredRotation = AxisValue < 0 ? FRotator(0.f, 180.f, 0.f) : FRotator(0.f, 0.f, 0.f);

	if (!DesiredRotation.Equals(LastSentRotation, 0.01f))
	{
		LastSentRotation = DesiredRotation;
		SetActorRotation(DesiredRotation);
		ServerSetDirection(DesiredRotation);
	}
}

void APlayerCharacter::ServerSetDirection_Implementation(const FRotator& Rotation)
{
	MulticastSetDirection(Rotation);
}

void APlayerCharacter::MulticastSetDirection_Implementation(const FRotator& Rotation)
{
	if (!IsLocallyControlled())
	{
		FRotator NormalRotation = Rotation.GetNormalized();
		SetActorRotation(NormalRotation);
	}
}

void APlayerCharacter::StartJump(const FInputActionValue& Value)
{
	if (bIsHit || !bIsAlive) return;
	ServerStartJump();
}

void APlayerCharacter::ServerStartJump_Implementation()
{
	MulticastStartJump();
}

void APlayerCharacter::MulticastStartJump_Implementation()
{
	if (JumpCurrentCount == 1)
	{
		bIsDoubleJump = true;
	}

	Jump();


	if (JumpSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, JumpSound, GetActorLocation());
	}
}

void APlayerCharacter::StopJump(const FInputActionValue& Value)
{
	if (!Value.Get<bool>())
	{
		//StopJumping();
	}
}

void APlayerCharacter::Dash(const FInputActionValue& Value)
{
	if (bIsHit || !bIsAlive) return;
	ServerDash();
}

void APlayerCharacter::ServerDash_Implementation()
{
	MulticastDash();
}

void APlayerCharacter::MulticastDash_Implementation()
{
	if (UCharacterMovementComponent* MoveComp = Cast<UCharacterMovementComponent>(GetMovementComponent()))
	{
		if (!MoveComp->IsFalling())
		{
			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

			if (AnimInstance && MapAnim["Dash"] && !AnimInstance->Montage_IsPlaying(MapAnim["Dash"]))
			{
				AnimInstance->StopAllMontages(1);
				AnimInstance->Montage_Play(MapAnim["Dash"]);
			}

			FVector Direction = GetMesh()->GetRightVector() * DashDistance;
			LaunchCharacter(Direction, true, false);
		}
	}
}

void APlayerCharacter::Guard(const FInputActionValue& Value)
{
	if (bIsHit || !bIsAlive) return;
	bool bIsGuard = Value.Get<bool>();

	TestTimer();

	// ªÁøÓµÂ ¿Áª˝
	if (!bIsGuarding && bIsGuard && GuardSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, GuardSound, GetActorLocation());
	}

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	UAnimMontage* AM = MapAnim["Guard"];

	if (AnimInstance && AM && !AnimInstance->Montage_IsPlaying(AM))
	{
		AnimInstance->StopAllMontages(1);
		AnimInstance->Montage_Play(AM);
	}

	bIsGuarding = true;
}

void APlayerCharacter::ReleaseGuard(const FInputActionValue& Value)
{
	if (bIsHit || !bIsAlive) return;
	bool bIsGuard = Value.Get<bool>();
	bIsGuarding = false;
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	UAnimMontage* AM = MapAnim["Guard"];

	if (AnimInstance && AM && !AnimInstance->Montage_IsPlaying(AM))
	{
		AnimInstance->Montage_Stop(0.1f, AM);
	}
}

void APlayerCharacter::NormalAttack(const FInputActionValue& Value)
{
	if (bIsHit || !bIsAlive) return;
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

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	UAnimMontage* NormalAttackMontage = NormalAttackMontages[NormalAttackMontageIndex % Size];

	if (AnimInstance && NormalAttackMontage && !AnimInstance->Montage_IsPlaying(NormalAttackMontages[PrevIndex % Size]))
	{
		AnimInstance->StopAllMontages(1);
		AnimInstance->Montage_Play(NormalAttackMontage);
	}

	if (NormalAttackSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, NormalAttackSound, GetActorLocation());
	}
}

void APlayerCharacter::Dead()
{
	ServerDead();
}

void APlayerCharacter::ServerDead_Implementation()
{
	MulticastDead();
}

void APlayerCharacter::MulticastDead_Implementation()
{
	bIsAlive = false;
}

void APlayerCharacter::BeginAttack()
{
	WeaponComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void APlayerCharacter::EndAttack()
{
	WeaponComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
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

	return BaseDamage * AttackerMultiplier / DefenderMultiplier;
}

void APlayerCharacter::Skill(const FInputActionValue& Value)
{
}

void APlayerCharacter::Ultimate(const FInputActionValue& Value)
{
}

void APlayerCharacter::ServerSkill_Implementation()
{
}

void APlayerCharacter::MulticastSkill_Implementation()
{
}

void APlayerCharacter::ServerUltimate_Implementation()
{
}

void APlayerCharacter::MulticastUltimate_Implementation()
{
}


void APlayerCharacter::SetHitState(bool IsHit)
{
	bIsHit = IsHit;
}

// =============
// Targeting Logic
// =============
APlayerCharacter* APlayerCharacter::GetTargetPlayer()
{
	FVector AttackOrigin; // Í≥µÍ≤© ?úÏûë ?ÑÏπò

	// Î¨¥Í∏∞Í∞Ä ?àÏúºÎ©?Î¨¥Í∏∞ ?åÏºì ?ÑÏπò?êÏÑú ?úÏûë?òÎèÑÎ°??§Ï†ï
	if (WeaponComponent && WeaponComponent->DoesSocketExist(FName("WeaponSocket")))
	{
		AttackOrigin = WeaponComponent->GetSocketLocation(FName("WeaponSocket"));
	}
	// Í∑∏Î†áÏßÄ ?äÏúºÎ©?Ï∫êÎ¶≠???ûÏ™Ω???ÑÏπò?òÎèÑÎ°??§Ï†ï
	else
	{
		AttackOrigin = GetActorLocation() + GetActorForwardVector() * ForwardOffset;
	}

	TArray<APlayerCharacter*> PotentialTargets = FindTargetsInRadius(AttackOrigin, AttackRadius); // Í≥µÍ≤© Î≤îÏúÑ ?¥Ïùò Î™®Îì† Ï∫êÎ¶≠?∞Î? Ï∞æÏùå
	APlayerCharacter* BestTarget = SelectBestTarget(PotentialTargets); // Í∞Ä???ÅÌï©???ÄÍ≤??†ÌÉù

	if (bDebugTargeting) // ?îÎ≤ÑÍ∑??úÍ∞Å??
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
	QueryParams.AddIgnoredActor(this); // ?êÏã†?Ä ?úÏô∏

	// Î¨ºÏ≤¥ ?§Î≤Ñ??Í≤Ä???òÌñâ
	bool bOverlapFound = GetWorld()->OverlapMultiByObjectType(
		OverlapResults,
		Origin,
		FQuat::Identity,
		FCollisionObjectQueryParams(TargetCollisionChannel), // ÏßÄ?ïÎêú Ï±ÑÎÑê???Ä??Í≤Ä??
		FCollisionShape::MakeSphere(Radius),
		QueryParams
	);

	if (bOverlapFound)
	{
		// ?§Î≤Ñ?©Îêú APlayerCharacter ?Ä?ÖÎßå ?ÑÌÑ∞Îß?
		for (const FOverlapResult& Overlap : OverlapResults)
		{
			APlayerCharacter* PlayerChar = Cast<APlayerCharacter>(Overlap.GetActor());
			if (PlayerChar && PlayerChar != this)
			{
				// ?Ä Í∏∞Îä• Íµ¨ÌòÑ???§Î•∏ ?ÄÎß??ÄÍ≤üÌåÖ?òÎèÑÎ°??ÑÌÑ∞Îß?Ï∂îÍ?
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

	// ?ÄÍ≤??†ÌÉù Í∏∞Ï?:
	// 1. ?∞ÏÑ†?úÏúÑ: ???úÏïºÍ∞??àÏóê ?àÎäî ??
	// 2. Í±∞Î¶¨: Í∞Ä??Í∞ÄÍπåÏö¥ ??

	APlayerCharacter* BestTarget = nullptr;
	float BestScore = -1.0f;

	FVector Forward = GetActorForwardVector();
	FVector ActorLocation = GetActorLocation();

	const float ViewAngleCos = FMath::Cos(FMath::DegreesToRadians(60.0f)); // 60???úÏïºÍ∞?
	const float ViewAngleWeight = 2.0f; // ?úÏïºÍ∞????ÅÏóê Í∞ÄÏ§ëÏπò

	for (APlayerCharacter* Target : PotentialTargets)
	{
		if (!Target) { continue; }

		FVector ToTarget = Target->GetActorLocation() - ActorLocation;
		float Distance = ToTarget.Size();

		float DistanceScore = 1.0f / FMath::Max(Distance, 1.0f); // Í±∞Î¶¨??Î∞òÎπÑÎ°Ä?òÎäî ?êÏàò Í≥ÑÏÇ∞ (Í∞ÄÍπåÏö∏?òÎ°ù ?íÏùå)

		ToTarget.Normalize(); // Î∞©Ìñ• ?ïÍ∑ú??

		float DotProduct = FVector::DotProduct(Forward, ToTarget); // ?ÑÎ∞© Î≤°ÌÑ∞?Ä ?ÄÍ≤?Î∞©Ìñ• Î≤°ÌÑ∞???¥Ï†Å Í≥ÑÏÇ∞

		float AngleScore = DotProduct > ViewAngleCos ? ViewAngleWeight : 1.0f; // ?úÏïºÍ∞??àÏóê ?àÎäîÏßÄ ?ïÏù∏ (DotProduct > ViewAngleCosÎ©??úÏïºÍ∞??àÏóê ?àÏùå)

		float FinalScore = DistanceScore * AngleScore; // ÏµúÏ¢Ö ?êÏàò Í≥ÑÏÇ∞

		// ??Ï¢ãÏ? ?êÏàò???ÄÍ≤üÏù¥Î©?Í∞±Ïã†
		if (FinalScore > BestScore)
		{
			BestScore = FinalScore;
			BestTarget = Target;
		}
	}
	return BestTarget;
}