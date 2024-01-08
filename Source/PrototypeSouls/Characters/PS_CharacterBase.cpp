#include "Characters/PS_CharacterBase.h"
#include "Components/PS_AbilitySystemComponent.h"
#include "GAS/Abilities/PS_GameplayAbility.h"
#include "Libraries/PS_NetLibrary.h"

UAbilitySystemComponent* APS_CharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UPS_PlayerAttributeSet* APS_CharacterBase::GetPlayerAttributeSet() const
{
	return PlayerAttributeSet;
}

void APS_CharacterBase::AddCharacterAbilities()
{
	if (!UPS_NetLibrary::IsServer(this) || !AbilitySystemComponent || AbilitySystemComponent->bAbilitiesGranted)
		return;

	for (TSubclassOf<UPS_GameplayAbility> Ability : Abilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Ability, 1, static_cast<int32>(Ability.GetDefaultObject()->InputID), this);
		const FGameplayTag& InputTag = Ability.GetDefaultObject()->InputTag;
		if (InputTag.IsValid())
		{
			AbilitySpec.DynamicAbilityTags.AddTag(InputTag);
		}
		AbilitySystemComponent->GiveAbility(AbilitySpec);
	}

	AbilitySystemComponent->bAbilitiesGranted = true;
}

void APS_CharacterBase::InitializeAttributes() const
{
	if (!AbilitySystemComponent || !DefaultAttributes)
		return;

	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	const FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(DefaultAttributes, 1, EffectContext);
	AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), AbilitySystemComponent);
}
