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
}

void AMainPlayerState::OnRep_Stock()
{
	UE_LOG(LogTemp, Warning, TEXT("✔️ OnRep_Stock called: %d"), Stock);
}
