#pragma once

#include "CoreMinimal.h"
#include "PS_GameplayAbility.h"
#include "PS_LockTargetAbility.generated.h"

UCLASS()
class UPS_LockTargetAbility : public UPS_GameplayAbility
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
		float Radius = 10.f;

	UPS_LockTargetAbility();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

};