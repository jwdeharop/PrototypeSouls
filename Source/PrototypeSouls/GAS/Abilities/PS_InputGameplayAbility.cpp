#include "GAS/Abilities/PS_InputGameplayAbility.h"
#include "Abilities/Tasks/AbilityTask_WaitInputRelease.h"

void UPS_InputGameplayAbility::OnRelease(float TimeHeld)
{
	ResetInputReleaseTask();
	K2_EndAbility();
}

void UPS_InputGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!K2_CommitAbility())
		return;
	
	ResetInputReleaseTask();

	WaitInputReleaseTask = UAbilityTask_WaitInputRelease::WaitInputRelease(this);
	WaitInputReleaseTask->OnRelease.AddUniqueDynamic(this, &ThisClass::OnRelease);
	WaitInputReleaseTask->ReadyForActivation();
}

void UPS_InputGameplayAbility::ResetInputReleaseTask()
{
	WaitInputReleaseTask.Reset();
	WaitInputReleaseTask = nullptr;
}
