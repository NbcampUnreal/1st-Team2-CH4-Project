#include "MainPlayerState.h"
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

