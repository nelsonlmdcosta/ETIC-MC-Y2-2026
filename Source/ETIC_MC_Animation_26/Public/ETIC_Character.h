// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ModifyHealth.h"

#include "ETIC_Character.generated.h"

struct FInputActionValue;
class UHealthComponent;

UCLASS(Blueprintable)
class ETIC_MC_ANIMATION_26_API AETIC_Character : public ACharacter, public IModifyHealth
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AETIC_Character();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	void PrintString();
	
private:
	
	UFUNCTION()
	void OnHealthAddedInputCallback(const FInputActionValue& Value);
	UFUNCTION()
	void OnHealthRemovedInputCallback(const FInputActionValue& Value);
	
private: // IModifyHealth Interface
	
	void DealDamage_Implementation(float amount) override;
	void HealDamage_Implementation(float amount) override;
	
protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	class UInputMappingContext* InputMappingContext;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	class UInputAction* HealthAddedAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	class UInputAction* HealthRemovedAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UHealthComponent* HealthComponent = nullptr;

};
