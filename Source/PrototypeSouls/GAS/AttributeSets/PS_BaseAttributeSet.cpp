#include "GAS/AttributeSets/PS_BaseAttributeSet.h"
#include "Characters/Player/PS_PlayerCharacter.h"
#include "UnrealNetwork.h"

void UPS_BaseAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION_NOTIFY(UPS_BaseAttributeSet, CurrentHealth, COND_None, REPNOTIFY_OnChanged);
	DOREPLIFETIME_CONDITION_NOTIFY(UPS_BaseAttributeSet, MaxWalkSpeed, COND_None, REPNOTIFY_OnChanged);
	DOREPLIFETIME_CONDITION_NOTIFY(UPS_BaseAttributeSet, Armor, COND_None, REPNOTIFY_OnChanged);
	DOREPLIFETIME_CONDITION_NOTIFY(UPS_BaseAttributeSet, Damage, COND_None, REPNOTIFY_OnChanged);
}

void UPS_BaseAttributeSet::OnRep_CurrentHealth(const FGameplayAttributeData& OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPS_BaseAttributeSet, CurrentHealth, OldHealth);
}

void UPS_BaseAttributeSet::OnRep_MaxWalkSpeed(const FGameplayAttributeData& OlMaxWalkSpeed) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPS_BaseAttributeSet, MaxWalkSpeed, OlMaxWalkSpeed);
}

void UPS_BaseAttributeSet::OnRep_CurrentArmor(const FGameplayAttributeData& OldArmor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPS_BaseAttributeSet, Armor, OldArmor);
}

void UPS_BaseAttributeSet::OnRep_Damage(const FGameplayAttributeData& OldDamage)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPS_BaseAttributeSet, Damage, OldDamage);
}
