#pragma once

#include "CoreMinimal.h"
#include "PS_GameplayAbility.h"
#include "PS_DodgeGameplayAbility.generated.h"

class APS_Character;

UCLASS()
class UPS_DodgeGameplayAbility : public UPS_GameplayAbility
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
		UAnimMontage* DodgeMontage = nullptr;
	UPROPERTY(EditDefaultsOnly)
		float DodgeSpeed = 900.f;

	UPS_DodgeGameplayAbility();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;
private:
	UFUNCTION()
		void OnCancelled();

	static FName GetDodgeSectionName(const APS_Character* APSCharacter);
	void OnAnimationCanStop(const APS_Character* APSCharacter);
};