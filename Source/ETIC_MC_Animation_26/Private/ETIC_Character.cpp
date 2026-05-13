// Fill out your copyright notice in the Description page of Project Settings.


#include "ETIC_Character.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "HealthComponent.h"

AETIC_Character::AETIC_Character()
{
	PrimaryActorTick.bCanEverTick = true;
	
	HealthComponent = CreateDefaultSubobject<UHealthComponent>("HealthComponent");
}

void AETIC_Character::BeginPlay()
{
	Super::BeginPlay();
	
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
	}
}
void AETIC_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
void AETIC_Character::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

// https://dev.epicgames.com/community/learning/tutorials/6dp3/unreal-engine-using-the-enhancedinput-system-in-c
void AETIC_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if (UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if(APlayerController* PlayerController = Cast<APlayerController>(GetController()))
		{
			if(UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			{
				Subsystem->AddMappingContext(InputMappingContext, 0);
			}		
		}
		
		Input->BindAction(HealthAddedAction, ETriggerEvent::Triggered, this, &AETIC_Character::OnHealthAddedInputCallback);
		Input->BindAction(HealthRemovedAction, ETriggerEvent::Triggered, this, &AETIC_Character::OnHealthRemovedInputCallback);
	}
}



void AETIC_Character::PrintString()
{
	GEngine->AddOnScreenDebugMessage(FName("SomeIdentifier").GetNumber(), 10.0f, FColor::Yellow, "Some Cool Message");

	int SomeInteger = 420;
	FString SomeString = "Henrique Iglesias";
	UE_LOG(LogTemp, Warning, TEXT("Hello I Have A Numebr %d That Is Named %s"), SomeInteger, *SomeString);
}

void AETIC_Character::OnHealthAddedInputCallback(const FInputActionValue& Value)
{
	// This case it's us but it could be any other actor that impelments this interface
	AActor* ActorExample = this;
	if (ActorExample->Implements<UModifyHealth>())
	{
		IModifyHealth::Execute_HealDamage(ActorExample, 10);
	}
}

void AETIC_Character::OnHealthRemovedInputCallback(const FInputActionValue& Value)
{
	// This case it's us but it could be any other actor that impelments this interface
	AActor* ActorExample = this;
	if (ActorExample->Implements<UModifyHealth>())
	{
		IModifyHealth::Execute_DealDamage(ActorExample, 10);
	}
}

void AETIC_Character::DealDamage_Implementation(float amount)
{
	HealthComponent->DealDamage(amount);
}

void AETIC_Character::HealDamage_Implementation(float amount)
{
	HealthComponent->HealDamage(amount);
}

