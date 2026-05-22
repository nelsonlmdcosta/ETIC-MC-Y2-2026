#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "CharacterAttributeSet.generated.h"

// ============================================================
//  Convenience macro — generates boilerplate Getter, Setter,
//  and Initter for each FGameplayAttributeData member.
//  Defined in AttributeSet.h by Epic.
// ============================================================
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName)           \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName)               \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName)               \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * UCharacterAttributeSet
 *
 * Holds attributes that are intrinsic to the CHARACTER regardless
 * of whether a weapon is equipped or not.
 *
 * Kept separate from UWeaponAttributeSet so that weapon attributes
 * can be granted / removed dynamically when a weapon is equipped
 * or unequipped — without touching character-level stats.
 *
 * Registered on the ASC at BeginPlay via:
 *   AbilitySystemComponent->GetOrCreateAttributeSubobject(UCharacterAttributeSet::StaticClass())
 */
UCLASS()
class TPS_TESTBED_API UCharacterAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:

	UCharacterAttributeSet();

	// --------------------------------------------------------
	//  Replication
	// --------------------------------------------------------

	/** Registers which attributes need to be replicated. */
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// --------------------------------------------------------
	//  Attribute clamping
	// --------------------------------------------------------

	/**
	 * Called just BEFORE an attribute's base value changes.
	 * Use this to clamp or redirect incoming values (e.g. damage
	 * meta-attribute pattern).
	 */
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	/**
	 * Called AFTER a Gameplay Effect changes an attribute.
	 * Use this to react to changes — e.g. kill the actor when
	 * Health reaches 0, or broadcast a UI event.
	 */
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	// --------------------------------------------------------
	//  Health
	// --------------------------------------------------------

	/** Current health. Clamped between 0 and MaxHealth. */
	UPROPERTY(BlueprintReadOnly, Category = "Character|Health", ReplicatedUsing = OnRep_Health)
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, Health)

	/** Maximum health. Drives the clamp ceiling for Health. */
	UPROPERTY(BlueprintReadOnly, Category = "Character|Health", ReplicatedUsing = OnRep_MaxHealth)
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, MaxHealth)

	/** Current health. Clamped between 0 and MaxHealth. */
	UPROPERTY(BlueprintReadOnly, Category = "Character|Mana", ReplicatedUsing = OnRep_MP)
	FGameplayAttributeData MP;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, MP)

	/** Maximum health. Drives the clamp ceiling for Health. */
	UPROPERTY(BlueprintReadOnly, Category = "Character|Mana", ReplicatedUsing = OnRep_MaxMP)
	FGameplayAttributeData MaxMP;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, MaxMP)
	
	/** Maximum health. Drives the clamp ceiling for Health. */
	UPROPERTY(BlueprintReadOnly, Category = "Character|Mana", ReplicatedUsing = OnRep_MPRegen)
	FGameplayAttributeData MPRegen;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, MPRegen)

protected:

	// --------------------------------------------------------
	//  RepNotifies
	//  These exist so clients can react to attribute changes
	//  (e.g. update health bars) without needing a separate RPC.
	// --------------------------------------------------------
	UFUNCTION()
	virtual void OnRep_Health(const FGameplayAttributeData& OldHealth);

	UFUNCTION()
	virtual void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth);
	
	UFUNCTION()
	virtual void OnRep_MP(const FGameplayAttributeData& OldHealth);

	UFUNCTION()
	virtual void OnRep_MaxMP(const FGameplayAttributeData& OldMaxHealth);
	
	UFUNCTION()
	virtual void OnRep_MPRegen(const FGameplayAttributeData& OldMaxHealth);
};