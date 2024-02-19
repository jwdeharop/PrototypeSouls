#include "Characters/Player/PS_PlayerState.h"
#include "Components/PS_AbilitySystemComponent.h"
#include "GAS/AttributeSets/PS_BaseAttributeSet.h"

APS_PlayerState::APS_PlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UPS_AbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	PlayerAttributeSet = CreateDefaultSubobject<UPS_BaseAttributeSet>(TEXT("AttributeSetBase"));

	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	
	NetUpdateFrequency = 100.0f;
}

UAbilitySystemComponent* APS_PlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UPS_BaseAttributeSet* APS_PlayerState::GetPlayerAttributeSet() const
{
	return PlayerAttributeSet;
}