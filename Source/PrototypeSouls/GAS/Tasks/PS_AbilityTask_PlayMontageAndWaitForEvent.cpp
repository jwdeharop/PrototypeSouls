#include "GAS/Tasks/PS_AbilityTask_PlayMontageAndWaitForEvent.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "GameFramework/Character.h"

UPS_AbilityTask_PlayMontageAndWaitForEvent* UPS_AbilityTask_PlayMontageAndWaitForEvent::PlayMontageAndWaitForEvent( UGameplayAbility* OwningAbility, FName TaskInstanceName, UAnimMontage* MontageToPlay, FGameplayTagContainer EventTags, float Rate, FName StartSection, bool bStopWhenAbilityEnds, float AnimRootMotionTranslationScale)
{
	UAbilitySystemGlobals::NonShipping_ApplyGlobalAbilityScaler_Rate(Rate);
	UPS_AbilityTask_PlayMontageAndWaitForEvent* MyObj = NewAbilityTask<UPS_AbilityTask_PlayMontageAndWaitForEvent>(OwningAbility, TaskInstanceName);
	if (MyObj)
	{
		MyObj->MontageToPlay = MontageToPlay;
		MyObj->EventTags = EventTags;
		MyObj->Rate = Rate;
		MyObj->StartSection = StartSection;
		MyObj->AnimRootMotionTranslationScale = AnimRootMotionTranslationScale;
		MyObj->bStopWhenAbilityEnds = bStopWhenAbilityEnds;
	}

	return MyObj;
	
}

void UPS_AbilityTask_PlayMontageAndWaitForEvent::Activate()
{
	if (!Ability)
		return;

	bool bMontagePlayed = false;
	if (AbilitySystemComponent.IsValid())
	{
		const FGameplayAbilityActorInfo* ActorInfo = Ability->GetCurrentActorInfo();
		if (UAnimInstance* AnimInstance = ActorInfo ? ActorInfo->GetAnimInstance() : nullptr)
		{
			EventHandle = AbilitySystemComponent->AddGameplayEventTagContainerDelegate(EventTags, FGameplayEventTagMulticastDelegate::FDelegate::CreateUObject(this, &UPS_AbilityTask_PlayMontageAndWaitForEvent::OnGameplayEvent));
			if (AbilitySystemComponent->PlayMontage(Ability, Ability->GetCurrentActivationInfo(), MontageToPlay, Rate, StartSection) > 0.f)
			{
				if (!ShouldBroadcastAbilityTaskDelegates())
					return;
				
				CancelledHandle = Ability->OnGameplayAbilityCancelled.AddUObject(this, &UPS_AbilityTask_PlayMontageAndWaitForEvent::OnAbilityCancelled);
				BlendingOutDelegate.BindUObject(this, &UPS_AbilityTask_PlayMontageAndWaitForEvent::OnMontageBlendingOut);
				AnimInstance->Montage_SetBlendingOutDelegate(BlendingOutDelegate, MontageToPlay);

				MontageEndedDelegate.BindUObject(this, &UPS_AbilityTask_PlayMontageAndWaitForEvent::OnMontageEnded);
				AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, MontageToPlay);

				if (ACharacter* Character = Cast<ACharacter>(GetAvatarActor()))
				{
					const bool bHasAuthority = Character->GetLocalRole() == ROLE_Authority;
					const bool bIsSimulated = Character->GetLocalRole() == ROLE_AutonomousProxy;
					const bool bIsLocalPredicted = Ability->GetNetExecutionPolicy() == EGameplayAbilityNetExecutionPolicy::LocalPredicted;
					if (bHasAuthority || (bIsSimulated && bIsLocalPredicted))
					{
						Character->SetAnimRootMotionTranslationScale(AnimRootMotionTranslationScale);
					}

					bMontagePlayed = true;
				}
				
			}
		}

		if (!bMontagePlayed)
		{
			if (ShouldBroadcastAbilityTaskDelegates())
			{
				OnCancelled.Broadcast(FGameplayTag(), FGameplayEventData());
			}
		}
	}

	SetWaitingOnAvatar();
}

void UPS_AbilityTask_PlayMontageAndWaitForEvent::ExternalCancel()
{
	OnAbilityCancelled();
	Super::ExternalCancel();
}

void UPS_AbilityTask_PlayMontageAndWaitForEvent::OnDestroy(bool AbilityEnded)
{
	if (Ability)
	{
		Ability->OnGameplayAbilityCancelled.Remove(CancelledHandle);
		if(AbilityEnded)
		{
			StopPlayingMontage();
		}
	}
	Super::OnDestroy(AbilityEnded);
}

bool UPS_AbilityTask_PlayMontageAndWaitForEvent::StopPlayingMontage()
{
	if (!AbilitySystemComponent.IsValid() || !Ability)
		return false;

	const FGameplayAbilityActorInfo* ActorInfo = Ability->GetCurrentActorInfo();
	const UAnimInstance* AnimInstance = ActorInfo ? ActorInfo->GetAnimInstance() : nullptr;
	if (!AnimInstance)
		return false;

	const bool bIsSameAbility = AbilitySystemComponent->GetAnimatingAbility() == Ability;
	const bool bIsSameMontage = AbilitySystemComponent->GetCurrentMontage() == MontageToPlay;
	if (bIsSameAbility && bIsSameMontage)
	{
		if (FAnimMontageInstance* MontageInstance = AnimInstance->GetActiveInstanceForMontage(MontageToPlay))
		{
			MontageInstance->OnMontageBlendingOutStarted.Unbind();
			MontageInstance->OnMontageEnded.Unbind();
		}

		AbilitySystemComponent->CurrentMontageStop();
		return true;
	}

	return false;
}

void UPS_AbilityTask_PlayMontageAndWaitForEvent::OnGameplayEvent(FGameplayTag GameplayTag, const FGameplayEventData* GameplayEventData)
{
}

void UPS_AbilityTask_PlayMontageAndWaitForEvent::OnAbilityCancelled()
{
}

void UPS_AbilityTask_PlayMontageAndWaitForEvent::OnMontageBlendingOut(UAnimMontage* AnimMontage, bool bInterrupted)
{
}

void UPS_AbilityTask_PlayMontageAndWaitForEvent::OnMontageEnded(UAnimMontage* AnimMontage, bool bInterrupted)
{
}
