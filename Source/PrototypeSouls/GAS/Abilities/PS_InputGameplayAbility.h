#pragma once

#include "CoreMinimal.h"
#include "GAS/Abilities/PS_GameplayAbility.h"
#include "PS_InputGameplayAbility.generated.h"

class UAbilityTask_WaitInputRelease;

UCLASS()
class UPS_InputGameplayAbility : public UPS_GameplayAbility
{
	GENERATED_BODY()

protected:
	UPROPERTY(Transient)
		TSoftObjectPtr<UAbilityTask_WaitInputRelease> WaitInputReleaseTask = nullptr;

	UFUNCTION()
		void OnRelease(float TimeHeld);

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

private:
	void ResetInputReleaseTask();
};