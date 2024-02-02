#pragma once

#include "CoreMinimal.h"
#include "PS_GameplayAbility.h"
#include "PS_DodgeGameplayAbility.generated.h"

class APS_PlayerCharacter;

UCLASS()
class UPS_DodgeGameplayAbility : public UPS_GameplayAbility
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
		UAnimMontage* DodgeMontage = nullptr;
	UPROPERTY(EditDefaultsOnly)
		UAnimMontage* RollMontage = nullptr;
	UPROPERTY(EditDefaultsOnly)
		float DodgeSpeed = 900.f;
	UPROPERTY(EditDefaultsOnly)
		float RollSpeed = 4000.f;

	UPS_DodgeGameplayAbility();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;

private:
	UFUNCTION()
		void OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData);
	UFUNCTION()
		void OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData);
	UFUNCTION()
		void OnEventReceived(FGameplayTag EventTag, FGameplayEventData EventData);
	UFUNCTION()
		void OnInputPressed(float TimeWaited);

	bool bIsRolling = false;
	static FName GetDodgeSectionName(const APS_PlayerCharacter* APSCharacter);
	static FName GetRollSection(const APS_PlayerCharacter* APSCharacter);
};