#pragma once

#include "CoreMinimal.h"
#include "AbilityTask.h"
#include "PS_AbilityTask_PlayMontageAndWaitForEvent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FPlayMontageAndWaitForEventDelegate, FGameplayTag, EventTag, FGameplayEventData, EventData);

UCLASS()
class UPS_AbilityTask_PlayMontageAndWaitForEvent : public UAbilityTask
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
		FPlayMontageAndWaitForEventDelegate OnCancelled;

	FOnMontageBlendingOutStarted BlendingOutDelegate;
	FOnMontageEnded MontageEndedDelegate;
	FDelegateHandle EventHandle;
	FDelegateHandle CancelledHandle;

	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
		static UPS_AbilityTask_PlayMontageAndWaitForEvent* PlayMontageAndWaitForEvent(UGameplayAbility* OwningAbility, FName TaskInstanceName, UAnimMontage* MontageToPlay, FGameplayTagContainer EventTags, float Rate = 1.f, FName StartSection = NAME_None, bool bStopWhenAbilityEnds = true, float AnimRootMotionTranslationScale = 1.f);

protected:
	virtual void Activate() override;
	virtual void ExternalCancel() override;
	virtual void OnDestroy(bool AbilityEnded) override;

private:
	UPROPERTY(Transient)
		FGameplayTagContainer EventTags;
	UPROPERTY(Transient)
		UAnimMontage* MontageToPlay;

	FName StartSection = NAME_None;
	float Rate = 1.f;
	float AnimRootMotionTranslationScale = 1.f;
	bool bStopWhenAbilityEnds = true;

	bool StopPlayingMontage();
	
	void OnGameplayEvent(FGameplayTag GameplayTag, const FGameplayEventData* GameplayEventData);
	void OnAbilityCancelled();
	void OnMontageBlendingOut(UAnimMontage* AnimMontage, bool bInterrupted);
	void OnMontageEnded(UAnimMontage* AnimMontage, bool bInterrupted);
};