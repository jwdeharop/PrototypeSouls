#include "GAS/Abilities/PS_DodgeGameplayAbility.h"
#include "AbilitySystemComponent.h"
#include "AbilityTask_WaitInputPress.h"
#include "Animations/PS_CharacterAnimInstance.h"
#include "Characters/Player/PS_PlayerCharacter.h"
#include "GAS/Tasks/PS_AbilityTask_PlayMontageAndWaitForEvent.h"
#include "Libraries/PS_NetLibrary.h"

UPS_DodgeGameplayAbility::UPS_DodgeGameplayAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UPS_DodgeGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	bIsRolling = false;
	APS_PlayerCharacter* APSCharacter = Cast<APS_PlayerCharacter>(ActorInfo->AvatarActor);
	const FName DodgeSection = GetDodgeSectionName(APSCharacter);
	if (!APSCharacter || DodgeSection == NAME_None)
	{
		K2_EndAbility();
		return;
	}

	const FGameplayTagContainer TagEvents;
	if (UPS_AbilityTask_PlayMontageAndWaitForEvent* PlayMontageAndWaitForEvent = UPS_AbilityTask_PlayMontageAndWaitForEvent::PlayMontageAndWaitForEvent(this, NAME_None, DodgeMontage, TagEvents, 1.f, DodgeSection))
	{
		PlayMontageAndWaitForEvent->OnBlendOut.AddUniqueDynamic(this, &ThisClass::OnCompleted);
		PlayMontageAndWaitForEvent->OnCompleted.AddUniqueDynamic(this, &ThisClass::OnCompleted);
		PlayMontageAndWaitForEvent->OnInterrupted.AddUniqueDynamic(this, &ThisClass::OnCancelled);
		PlayMontageAndWaitForEvent->OnCancelled.AddUniqueDynamic(this, &ThisClass::OnCancelled);
		PlayMontageAndWaitForEvent->OnEventReceived.AddUniqueDynamic(this, &ThisClass::OnEventReceived);
		PlayMontageAndWaitForEvent->ReadyForActivation();
	}

	if (UPS_NetLibrary::IsServer(this))
	{
		const FVector2d& NormalizedMovement = APSCharacter->AuxMovementVector;
		const FVector ForwardVector = (APSCharacter->GetActorForwardVector() * NormalizedMovement.Y) + (APSCharacter->GetActorRightVector() * NormalizedMovement.X);
		APSCharacter->LaunchCharacter(ForwardVector * DodgeSpeed, false, false);
	}
}

bool UPS_DodgeGameplayAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

void UPS_DodgeGameplayAbility::OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData)
{
	K2_EndAbility();
}

void UPS_DodgeGameplayAbility::OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData)
{
	if (!bIsRolling)
	{
		K2_EndAbility();
	}
}

void UPS_DodgeGameplayAbility::OnEventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{
	if (EventTag == FGameplayTag::RequestGameplayTag("Ability.Dodge.CanRoll"))
	{
		// If can roll, we create another task waiting for the input.
		if (UAbilityTask_WaitInputPress* WaitInputPress = UAbilityTask_WaitInputPress::WaitInputPress(this))
		{
			WaitInputPress->OnPress.AddUniqueDynamic(this, &ThisClass::OnInputPressed);
			WaitInputPress->ReadyForActivation();
		}
		return;
	}

	if (EventTag == FGameplayTag::RequestGameplayTag("Ability.Dodge.CanInterruptDodge"))
	{
		if (!bIsRolling)
		{
			const APS_PlayerCharacter* PlayerCharacter = Cast<APS_PlayerCharacter>(GetAvatarActorFromActorInfo());
			if (PlayerCharacter && PlayerCharacter->IsMoving())
			{
				PlayerCharacter->GetAbilitySystemComponent()->CurrentMontageStop();
				K2_EndAbility();
			}
		}
	}
}

void UPS_DodgeGameplayAbility::OnInputPressed(float TimeWaited)
{
	APS_PlayerCharacter* APSCharacter = Cast<APS_PlayerCharacter>(GetAvatarActorFromActorInfo());
	if (!APSCharacter)
		return;

	bIsRolling = true;

	APSCharacter->GetAbilitySystemComponent()->StopMontageIfCurrent(*DodgeMontage);

	if (UPS_AbilityTask_PlayMontageAndWaitForEvent* PlayMontageAndWaitForEvent = UPS_AbilityTask_PlayMontageAndWaitForEvent::PlayMontageAndWaitForEvent(this, NAME_None, RollMontage, FGameplayTagContainer(), 1.f, GetRollSection(APSCharacter), true))
	{
		PlayMontageAndWaitForEvent->OnBlendOut.AddUniqueDynamic(this, &ThisClass::OnCompleted);
		PlayMontageAndWaitForEvent->OnCompleted.AddUniqueDynamic(this, &ThisClass::OnCompleted);
		PlayMontageAndWaitForEvent->OnInterrupted.AddUniqueDynamic(this, &ThisClass::OnCancelled);
		PlayMontageAndWaitForEvent->OnCancelled.AddUniqueDynamic(this, &ThisClass::OnCancelled);
		PlayMontageAndWaitForEvent->OnEventReceived.AddUniqueDynamic(this, &ThisClass::OnEventReceived);
		PlayMontageAndWaitForEvent->ReadyForActivation();
	}

	if (UPS_NetLibrary::IsServer(this))
	{
		const FVector2d& NormalizedMovement = APSCharacter->AuxMovementVector;
		const FVector ForwardVector = (APSCharacter->GetActorForwardVector() * NormalizedMovement.Y) + (APSCharacter->GetActorRightVector() * NormalizedMovement.X);
		APSCharacter->LaunchCharacter(ForwardVector * RollSpeed, false, false);
	}
}

FName UPS_DodgeGameplayAbility::GetDodgeSectionName(const APS_PlayerCharacter* APSCharacter)
{
	const USkeletalMeshComponent* Mesh = APSCharacter ? APSCharacter->GetMesh() : nullptr;
	UPS_CharacterAnimInstance* AnimInstance = Mesh ? Cast<UPS_CharacterAnimInstance>(Mesh->GetAnimInstance()) : nullptr;
	if (!AnimInstance)
		return NAME_None;

	const EPS_AnimationDirection AnimationDirection = AnimInstance->GetAnimationDirection();
	switch (AnimationDirection)
	{
		case EPS_AnimationDirection::Front:
			return "front";
		case EPS_AnimationDirection::Left:
			return "left";
		case EPS_AnimationDirection::Right:
			return "right";
		case EPS_AnimationDirection::BackLeft:
			return "bwd_l";
		case EPS_AnimationDirection::BackRight:
			return "bwd_r";
		case EPS_AnimationDirection::FrontLeft:
			return "front_l";
		case EPS_AnimationDirection::FrontRight:
			return "front_r";
		default:
			return "bwd";
	}
}

FName UPS_DodgeGameplayAbility::GetRollSection(const APS_PlayerCharacter* APSCharacter)
{
	const FVector2D& InputMovement = APSCharacter ? APSCharacter->AuxMovementVector : FVector2d::Zero();
	if (InputMovement.IsNearlyZero())
		return NAME_None;

	const float RightAxis = FMath::Abs(InputMovement.X);
	const float UpAxis = FMath::Abs(InputMovement.Y);

	if (RightAxis > UpAxis)
	{
		if (InputMovement.X > 0.f)
		{
			return "right";
		}

		return "left";
	}

	
	if (InputMovement.Y > 0.f)
	{
		return "front";
	}

	return "bwd";
}
