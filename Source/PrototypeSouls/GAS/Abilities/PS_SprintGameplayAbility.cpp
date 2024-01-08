#include "GAS/Abilities/PS_SprintGameplayAbility.h"
#include "Abilities/Tasks/AbilityTask_WaitInputRelease.h"
#include "Characters/Player/PS_PlayerCharacter.h"
#include "Components/PS_AbilitySystemComponent.h"
#include "Libraries/PS_NetLibrary.h"

void UPS_SprintGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		K2_EndAbility();
		return;
	}

	if (UPS_NetLibrary::IsServer(this))
	{
		SprintGEHandle = ApplyGameplayEffectToOwner(Handle, ActorInfo, ActivationInfo, SprintGameplayEffect.GetDefaultObject(), 1);
	}
}

void UPS_SprintGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	if (UPS_NetLibrary::IsServer(this))
	{
		const APS_PlayerCharacter* APSCharacter = Cast<APS_PlayerCharacter>(ActorInfo->AvatarActor);
		UPS_AbilitySystemComponent* AbilitySystemComponent = APSCharacter ? Cast<UPS_AbilitySystemComponent>(APSCharacter->GetAbilitySystemComponent()) : nullptr;
		if (!AbilitySystemComponent)
			return;

		BP_RemoveGameplayEffectFromOwnerWithHandle(SprintGEHandle);
	}
}
