#pragma once

#include "CoreMinimal.h"
#include "GAS/Abilities/PS_GameplayAbility.h"
#include "PS_LightAttackAbility.generated.h"

class APS_PlayerCharacter;

UCLASS()
class UPS_LightAttackAbility : public UPS_GameplayAbility
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TSubclassOf<UGameplayEffect> DamageGameplayEffect;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

private:
	UPROPERTY(Transient)
		APS_PlayerCharacter* APSCharacter = nullptr;

	FTimerHandle OnCanChangeYawTimer;

	UFUNCTION()
		void OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData);
	UFUNCTION()
		void OnPressed(float TimeWaited);
	UFUNCTION()
		void OnEventReceived(FGameplayTag EventTag, FGameplayEventData EventData);

	bool PlayCurrentCombo();
	void BindToPressEvent();
};