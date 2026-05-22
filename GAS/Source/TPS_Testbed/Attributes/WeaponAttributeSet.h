#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "WeaponAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName)           \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName)               \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName)               \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * UWeaponAttributeSet
 *
 * Holds attributes that belong to an EQUIPPED WEAPON.
 *
 * Key design point for students:
 * This AttributeSet is NOT added at BeginPlay. Instead it is
 * granted to the ASC dynamically when a weapon is equipped and
 * removed when the weapon is unequipped. This keeps weapon stats
 * completely decoupled from the character.
 *
 * Grant pattern (called from the weapon or equipment manager):
 *
 *   // Equip
 *   FGameplayAbilitySpec WeaponSpec = ...;
 *   ASC->GiveAbility(WeaponSpec);
 *   WeaponAttributeSet = NewObject<UWeaponAttributeSet>(ASC->GetOwner());
 *   ASC->AddAttributeSetSubobject(WeaponAttributeSet);
 *   ASC->ApplyGameplayEffectToSelf(GE_WeaponDefaults, ...); // sets initial values
 *
 *   // Unequip
 *   ASC->RemoveSpawnedAttribute(WeaponAttributeSet);
 *   WeaponAttributeSet = nullptr;
 */
UCLASS()
class TPS_TESTBED_API UWeaponAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:

	UWeaponAttributeSet();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	// --------------------------------------------------------
	//  Attack Power
	//
	//  Flat damage contribution from this weapon.
	//  Referenced by GE_WarpstrikeDamage as a Backing Attribute
	//  so damage scales automatically with weapon upgrades.
	// --------------------------------------------------------
	UPROPERTY(BlueprintReadOnly, Category = "Weapon|Damage", ReplicatedUsing = OnRep_AttackPower)
	FGameplayAttributeData AttackPower;
	ATTRIBUTE_ACCESSORS(UWeaponAttributeSet, AttackPower)
	
	// --------------------------------------------------------
	//  Warpstrike Range
	//
	//  Maximum distance (cm) at which the Warpstrike ability
	//  can lock on to a target. Driven by this attribute so
	//  weapon upgrades or buffs can extend it via GEs without
	//  touching ability code.
	// --------------------------------------------------------
	UPROPERTY(BlueprintReadOnly, Category = "Weapon|Warpstrike", ReplicatedUsing = OnRep_WarpstrikeRange)
	FGameplayAttributeData WarpstrikeRange;
	ATTRIBUTE_ACCESSORS(UWeaponAttributeSet, WarpstrikeRange)

protected:

	UFUNCTION()
	virtual void OnRep_AttackPower(const FGameplayAttributeData& OldAttackPower);

	UFUNCTION()
	virtual void OnRep_WarpstrikeRange(const FGameplayAttributeData& OldWarpstrikeRange);
};