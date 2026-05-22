// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"

UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	
	if (ShouldInitializeToMaxHealth)
		CurrentHealth = MaxHealth;
}

void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UHealthComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void UHealthComponent::DealDamage(float amount)
{
	if (CurrentHealth <= 0.0f)
		return;
	
	CurrentHealth = FMath::Clamp( CurrentHealth - amount, 0.0f, MaxHealth );

	OnDamageReceivedEvent.Broadcast(GetNormalizedHealth());

	if (CurrentHealth == 0.0f) 
	{
		OnDeathEvent.Broadcast();
	}
}


void UHealthComponent::HealDamage(float amount)
{
	if (CurrentHealth <= 0.0f)
		return;

	CurrentHealth = FMath::Clamp( CurrentHealth + amount, 0.0f, MaxHealth );
	
	OnDamageHealedEvent.Broadcast(GetNormalizedHealth());
}

float UHealthComponent::GetCurrentHealth() const
{
	return CurrentHealth;
}

float UHealthComponent::GetMaxHealth() const
{
	return MaxHealth;
}

float UHealthComponent::GetNormalizedHealth() const
{
	return CurrentHealth / MaxHealth;
}

