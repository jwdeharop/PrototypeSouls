#include "GAS/Abilities/PS_DodgeGameplayAbility.h"
#include "AbilityTask_PlayMontageAndWait.h"
#include "AbilityTask_WaitInputPress.h"
#include "Animations/PS_CharacterAnimInstance.h"
#include "Characters/PS_PlayerCharacter.h"

UPS_DodgeGameplayAbility::UPS_DodgeGameplayAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UPS_DodgeGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	APS_PlayerCharacter* APSCharacter = Cast<APS_PlayerCharacter>(ActorInfo->AvatarActor);
	const FName DodgeSection = GetDodgeSectionName(APSCharacter);
	if (!APSCharacter || DodgeSection == NAME_None)
	{
		K2_EndAbility();
		return;
	}

	UPS_CharacterAnimInstance* AnimInstance = CastChecked<UPS_CharacterAnimInstance>(APSCharacter->GetMesh()->GetAnimInstance());
	if (AnimInstance)
	{
		AnimInstance->AnimationCanStop.AddUObject(this, &ThisClass::OnAnimationCanStop);
	}

	if (UAbilityTask_PlayMontageAndWait* PlayMontageAndWaitTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, DodgeMontage, 1.f, DodgeSection))
	{
		PlayMontageAndWaitTask->OnCancelled.AddUniqueDynamic(this, &ThisClass::OnCancelled);
		PlayMontageAndWaitTask->OnCompleted.AddUniqueDynamic(this, &ThisClass::OnCancelled);
		PlayMontageAndWaitTask->OnInterrupted.AddUniqueDynamic(this, &ThisClass::OnCancelled);
		PlayMontageAndWaitTask->OnBlendOut.AddUniqueDynamic(this, &ThisClass::OnCancelled);
		PlayMontageAndWaitTask->ReadyForActivation();
	}

	const FVector2d& NormalizedMovement = APSCharacter->AuxMovementVector;
	const FVector ForwardVector = (APSCharacter->GetActorForwardVector() * NormalizedMovement.Y) + (APSCharacter->GetActorRightVector() * NormalizedMovement.X);
	APSCharacter->LaunchCharacter(ForwardVector * DodgeSpeed, false, false);
}

bool UPS_DodgeGameplayAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

void UPS_DodgeGameplayAbility::OnCancelled()
{
	K2_EndAbility();
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

void UPS_DodgeGameplayAbility::OnAnimationCanStop(const APS_PlayerCharacter* APSCharacter)
{
	UPS_CharacterAnimInstance* AnimInstance = APSCharacter ? CastChecked<UPS_CharacterAnimInstance>(APSCharacter->GetMesh()->GetAnimInstance()) : nullptr;
	if (!AnimInstance)
	{
		K2_EndAbility();
		return;
	}

	AnimInstance->Montage_Stop(0.2f, DodgeMontage);
	K2_EndAbility();
}
