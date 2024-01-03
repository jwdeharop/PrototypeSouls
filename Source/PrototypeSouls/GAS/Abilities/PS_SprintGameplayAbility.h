#pragma once

#include "CoreMinimal.h"
#include "GAS/Abilities/PS_InputGameplayAbility.h"
#include "PS_SprintGameplayAbility.generated.h"

UCLASS()
class UPS_SprintGameplayAbility : public UPS_InputGameplayAbility
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "ASC")
		TSubclassOf<UGameplayEffect> SprintGameplayEffect;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

private:
	FActiveGameplayEffectHandle SprintGEHandle;
};