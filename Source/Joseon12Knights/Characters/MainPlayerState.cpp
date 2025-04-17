#include "MainPlayerState.h"
#include "Net/UnrealNetwork.h"
#include "PlayerCharacter.h"
#include "MainPlayerController.h"

AMainPlayerState::AMainPlayerState()
{
	MaxHealth = 100.f;
	CurrentHealth = MaxHealth;
}

void AMainPlayerState::Initialize()
{
	UE_LOG(LogTemp, Warning, TEXT("Init Player State"));	
}

float AMainPlayerState::GetPlayerHealth() const
{
	return CurrentHealth;
}

void AMainPlayerState::SetPlayerStatus(float Health)
{
	MaxHealth = Health;
	CurrentHealth = MaxHealth;
}

void AMainPlayerState::SetDamage(float Damage)
{
	CurrentHealth -= Damage;
}

void AMainPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AMainPlayerState, Stock);
	DOREPLIFETIME(AMainPlayerState, CurrentHealth);
	DOREPLIFETIME(AMainPlayerState, MaxHealth);

}

void AMainPlayerState::OnRep_Stock()
{
	UE_LOG(LogTemp, Warning, TEXT("✔️ OnRep_Stock called: %d"), Stock);
}


void AMainPlayerState::OnRep_CurrentHealth()
{
	UE_LOG(LogTemp, Warning, TEXT("🩸 체력 변경됨: %.1f"), CurrentHealth);
}

void AMainPlayerState::OnRep_MaxHealth()
{
	UE_LOG(LogTemp, Warning, TEXT("🧱 최대 체력 변경됨: %.1f"), MaxHealth);
}
