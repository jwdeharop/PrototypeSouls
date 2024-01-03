#include "Components/PS_AbilitySystemComponent.h"

void UPS_AbilitySystemComponent::AbilityInputTagPressed(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid())
		return;

	const FGameplayAbilitySpec* AbilitySpec = GetActivatableAbilities().FindByPredicate([&InputTag](const FGameplayAbilitySpec& Spec)
	{
		return Spec.DynamicAbilityTags.HasTag(InputTag);
	});

	if (AbilitySpec)
	{
		InputPressedSpecHandles.AddUnique(AbilitySpec->Handle);
	}
}

void UPS_AbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid())
		return;

	const FGameplayAbilitySpec* AbilitySpec = GetActivatableAbilities().FindByPredicate([&InputTag](const FGameplayAbilitySpec& Spec)
	{
		return Spec.DynamicAbilityTags.HasTag(InputTag);
	});

	if (AbilitySpec)
	{
		InputReleasedSpecHandles.AddUnique(AbilitySpec->Handle);
	}
}

void UPS_AbilitySystemComponent::ProcessAbilityInput(const float DeltaTime, const bool bGamePaused)
{
	static TArray<FGameplayAbilitySpecHandle, TInlineAllocator<4>> AbilitiesToActivate;
	AbilitiesToActivate.Reset();

	for (const FGameplayAbilitySpecHandle& InputPressedSpecHandle : InputPressedSpecHandles)
	{
		FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(InputPressedSpecHandle);
		if (!AbilitySpec)
			continue;

		if (AbilitySpec->IsActive())
		{
			AbilitySpecInputPressed(*AbilitySpec);
		}
		else
		{
			AbilitiesToActivate.Add(AbilitySpec->Handle);
		}
	}

	for (const FGameplayAbilitySpecHandle& AbilityToActivate : AbilitiesToActivate)
	{
		const FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(AbilityToActivate);
		if (!AbilitySpec)
			continue;

		TryActivateAbility(AbilityToActivate);
	}

	for (const FGameplayAbilitySpecHandle& InputReleasedSpecHandle : InputReleasedSpecHandles)
	{
		if (FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(InputReleasedSpecHandle))
		{
			if (AbilitySpec->IsActive())
			{
				AbilitySpecInputReleased(*AbilitySpec);
			}
		}
	}

	ClearAbilityInput();
}

void UPS_AbilitySystemComponent::AbilitySpecInputPressed(FGameplayAbilitySpec& Spec)
{
	Super::AbilitySpecInputPressed(Spec);

	if (Spec.IsActive())
	{
		InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed, Spec.Handle, Spec.ActivationInfo.GetActivationPredictionKey());
	}
}

void UPS_AbilitySystemComponent::AbilitySpecInputReleased(FGameplayAbilitySpec& Spec)
{
	Super::AbilitySpecInputReleased(Spec);

	if (Spec.IsActive())
	{
		InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased, Spec.Handle, Spec.ActivationInfo.GetActivationPredictionKey());
	}
}

void UPS_AbilitySystemComponent::ClearAbilityInput()
{
	InputReleasedSpecHandles.Reset();
	InputPressedSpecHandles.Reset();
}
