#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "PS_AbilitySystemComponent.generated.h"

UCLASS()
class UPS_AbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	bool bAbilitiesGranted = false;

	void AbilityInputTagPressed(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);

	void ProcessAbilityInput(const float DeltaTime, const bool bGamePaused);

protected:
	virtual void AbilitySpecInputPressed(FGameplayAbilitySpec& Spec) override;
	virtual void AbilitySpecInputReleased(FGameplayAbilitySpec& Spec) override;

private:
	TArray<FGameplayAbilitySpecHandle> InputPressedSpecHandles;
	TArray<FGameplayAbilitySpecHandle> InputReleasedSpecHandles;

	void ClearAbilityInput();
};

