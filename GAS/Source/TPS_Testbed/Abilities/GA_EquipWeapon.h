// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Attributes/WeaponAttributeSet.h"
#include "Abilities/GameplayAbility.h"
#include "GA_EquipWeapon.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class TPS_TESTBED_API UGA_EquipWeapon : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	TSubclassOf<UAttributeSet> AttributesToAdd;
	
	UPROPERTY()
	UWeaponAttributeSet* WeaponAttributes;
	
	UPROPERTY()
	UStaticMeshComponent* MeshToAdd;
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
};
