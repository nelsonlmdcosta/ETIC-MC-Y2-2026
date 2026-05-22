// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_EquipWeapon.h"

void UGA_EquipWeapon::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	// CreateDefaultSubobject CANNOT be used here, that is specifically for constructor based calls, here we use NewObject and link it in
	WeaponAttributes = NewObject<UWeaponAttributeSet>(this, "WeaponAttributes");
	ActorInfo->AbilitySystemComponent->AddSpawnedAttribute(WeaponAttributes);
	
	// TOO: Spawn Mesh And Attach It To A Socket
}
