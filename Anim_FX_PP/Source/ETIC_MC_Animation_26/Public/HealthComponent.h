// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <stdbool.h>

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDamageReceived, float, NormalizedHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDamageHealed, float, NormalizedHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeath);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ETIC_MC_ANIMATION_26_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UHealthComponent();

public:
	
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION(BlueprintCallable)
	void DealDamage(float amount);
	UFUNCTION(BlueprintCallable)
	void HealDamage(float amount);

	UFUNCTION(BlueprintPure)
	float GetCurrentHealth() const;
	UFUNCTION(BlueprintPure)
	float GetMaxHealth() const;
	UFUNCTION(BlueprintPure)
	float GetNormalizedHealth() const;
	
public: // Events
	
	UPROPERTY(BlueprintAssignable)
	FOnDamageReceived	OnDamageReceivedEvent;
	UPROPERTY(BlueprintAssignable)
	FOnDamageHealed		OnDamageHealedEvent;
	UPROPERTY(BlueprintAssignable)
	FOnDeath			OnDeathEvent;
	
private:

	UPROPERTY(EditAnywhere)
	float CurrentHealth = 50.0f;
	UPROPERTY(EditAnywhere)
	float MaxHealth = 100.0f;
	UPROPERTY(EditAnywhere)
	bool ShouldInitializeToMaxHealth = true;
};
