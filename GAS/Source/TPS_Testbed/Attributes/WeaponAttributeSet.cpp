#include "WeaponAttributeSet.h"
#include "Net/UnrealNetwork.h"
#include "GameplayEffectExtension.h"

// ============================================================
//  Constructor
// ============================================================
UWeaponAttributeSet::UWeaponAttributeSet()
{
	// Defaults represent a bare, unupgraded weapon state.
	// In practice these will always be overwritten by a
	// GE_WeaponDefaults effect applied when the weapon is equipped,
	// so these are just safety fallbacks.
	InitAttackPower(25.f);
	InitWarpstrikeRange(1500.f); // 15 metres
}

// ============================================================
//  Replication
// ============================================================
void UWeaponAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UWeaponAttributeSet, AttackPower,      COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UWeaponAttributeSet, WarpstrikeRange,  COND_None, REPNOTIFY_Always);
}

// ============================================================
//  PreAttributeChange
// ============================================================
void UWeaponAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetAttackPowerAttribute())
	{
		// Attack power should never go negative — a debuff can reduce
		// it to 0 (pacified weapon) but not below.
		NewValue = FMath::Max(NewValue, 0.f);
	}
	else if (Attribute == GetWarpstrikeRangeAttribute())
	{
		// Minimum meaningful range — prevents locking on to targets
		// that are literally inside the character's collision capsule.
		NewValue = FMath::Max(NewValue, 50.f);
	}
}

// ============================================================
//  PostGameplayEffectExecute
//
//  Weapon attributes are generally straightforward — no death
//  checks needed. This is mostly here as the correct pattern
//  for students to extend if they add more complex attributes
//  (e.g. a Durability attribute that breaks the weapon at 0).
// ============================================================
void UWeaponAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetAttackPowerAttribute())
	{
		// Re-clamp after all modifiers have been applied.
		SetAttackPower(FMath::Max(GetAttackPower(), 0.f));
	}
	else if (Data.EvaluatedData.Attribute == GetWarpstrikeRangeAttribute())
	{
		SetWarpstrikeRange(FMath::Max(GetWarpstrikeRange(), 50.f));
	}
}

// ============================================================
//  RepNotifies
// ============================================================
void UWeaponAttributeSet::OnRep_AttackPower(const FGameplayAttributeData& OldAttackPower)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UWeaponAttributeSet, AttackPower, OldAttackPower);
}

void UWeaponAttributeSet::OnRep_WarpstrikeRange(const FGameplayAttributeData& OldWarpstrikeRange)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UWeaponAttributeSet, WarpstrikeRange, OldWarpstrikeRange);
}