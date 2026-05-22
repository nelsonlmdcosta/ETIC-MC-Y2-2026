#include "CharacterAttributeSet.h"
#include "Net/UnrealNetwork.h"
#include "GameplayEffectExtension.h"
#include "GameFramework/Character.h"

UCharacterAttributeSet::UCharacterAttributeSet()
{
	// Set sensible defaults. These will typically be overridden
	// by a startup Gameplay Effect (GE_DefaultStats) applied on
	// BeginPlay, but defaults prevent divide by zero issues and
	// ensure newly spawned characters are in a valid state.
	InitHealth(100.f);
	InitMaxHealth(100.f);
	InitMP(50.0f);
	InitMaxMP(50.0f);
}

// ============================================================
//  Replication
// ============================================================
void UCharacterAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// COND_None = replicate to all connections that own or observe this actor.
	DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSet, Health,		COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSet, MaxHealth,	COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSet, MP,			COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSet, MaxMP,		COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSet, MPRegen,		COND_None, REPNOTIFY_Always);
}

// ============================================================
//  PreAttributeChange
//
//  This fires for BOTH base value and current value changes.
//  It does NOT know about the GE that caused the change yet
//  use PostGameplayEffectExecute for GE-aware logic.
// ============================================================
void UCharacterAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute())
	{
		// Hard clamp Health can never go below 0 or above MaxHealth.
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}
	else if (Attribute == GetMaxHealthAttribute())
	{
		// MaxHealth should never be negative.
		NewValue = FMath::Max(NewValue, 1.f);
	}
	else if (Attribute == GetMPAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxMP());
	}
	else if (Attribute == GetMaxMPAttribute())
	{
		NewValue = FMath::Max(NewValue, 1.f);
	}
}

// ============================================================
//  Post GE Execute
//
//  Fires AFTER a Gameplay Effect has modified an attribute.
//  This is the correct place for death checks, UI events, etc.
// ============================================================
void UCharacterAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		// Re-clamp after the GE has applied its full modifier.
		// PreAttributeChange alone isn't enough because GEs can
		// apply additive + multiplicative stacks in sequence.
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));

		// TODO: If Health == 0, notify the owning character to trigger
		// a death ability or broadcast an OnDeath event here.
		if (GetHealth() <= 0.f)
		{
		    ACharacter* Owner = Cast<ACharacter>(GetOwningActor());
		    //if (Owner && Owner->Implements<>())
		    //{
			//    Owner->HandleDeath();
		    //}
		}
	}
	
	if (Data.EvaluatedData.Attribute == GetMPAttribute())
	{
		SetMP(FMath::Clamp(GetMP(), 0.f, GetMaxMP()));
	}
}

// ============================================================
//  RepNotifies
//
//  On clients, these fire after the replicated value arrives.
//  The OldValue parameter lets you calculate deltas for
//  animated health bars, floating combat text, etc.
// ============================================================
void UCharacterAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	// This macro notifies the ASC on the client that Health changed
	// so that Gameplay Cues and tag requirements stay in sync.
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, Health, OldHealth);
}

void UCharacterAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, MaxHealth, OldMaxHealth);
}

void UCharacterAttributeSet::OnRep_MP(const FGameplayAttributeData& OldMP)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, MP, OldMP);
}

void UCharacterAttributeSet::OnRep_MaxMP(const FGameplayAttributeData& OldMaxMP)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, MaxMP, OldMaxMP);
}

void UCharacterAttributeSet::OnRep_MPRegen(const FGameplayAttributeData& OldMaxMP)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, MPRegen, OldMaxMP);
}