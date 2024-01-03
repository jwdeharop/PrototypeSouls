#include "GAS/AttributeSets/PS_PlayerAttributeSet.h"

#include "UnrealNetwork.h"

void UPS_PlayerAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION_NOTIFY(UPS_PlayerAttributeSet, CurrentSpeed, COND_None, REPNOTIFY_OnChanged);
	DOREPLIFETIME_CONDITION_NOTIFY(UPS_PlayerAttributeSet, CurrentHealth, COND_None, REPNOTIFY_OnChanged);
	DOREPLIFETIME_CONDITION_NOTIFY(UPS_PlayerAttributeSet, MaxWalkSpeed, COND_None, REPNOTIFY_OnChanged);
}

void UPS_PlayerAttributeSet::OnRep_CurrentSpeed(const FGameplayAttributeData& OldMoveSpeed) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPS_PlayerAttributeSet, CurrentSpeed, OldMoveSpeed);
}

void UPS_PlayerAttributeSet::OnRep_CurrentHealth(const FGameplayAttributeData& OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPS_PlayerAttributeSet, CurrentHealth, OldHealth);
}

void UPS_PlayerAttributeSet::OnRep_MaxWalkSpeed(const FGameplayAttributeData& OlMaxWalkSpeed) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPS_PlayerAttributeSet, MaxWalkSpeed, OlMaxWalkSpeed);
}
