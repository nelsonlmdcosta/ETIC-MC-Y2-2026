// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ModifyHealth.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UModifyHealth : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ETIC_MC_ANIMATION_26_API IModifyHealth
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	
	UFUNCTION(BlueprintNativeEvent, Category="ModifyHealth")
	void DealDamage(float amount);

	UFUNCTION(BlueprintNativeEvent, Category="ModifyHealth")
	void HealDamage(float amount);

	// Specifically just for blueprint to implement
	//UFUNCTION(BlueprintImplementableEvent)
	
};
