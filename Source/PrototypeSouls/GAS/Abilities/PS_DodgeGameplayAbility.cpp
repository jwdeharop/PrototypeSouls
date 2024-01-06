#include "GAS/Abilities/PS_DodgeGameplayAbility.h"
#include "AbilityTask_PlayMontageAndWait.h"
#include "AbilityTask_WaitInputPress.h"
#include "Animations/PS_CharacterAnimInstance.h"
#include "Characters/PS_Character.h"
#include "Kismet/KismetMathLibrary.h"

namespace UPS_DodgeGameplayAbility_Const
{
	static const TMap<FVector2d, FName> Dodges = {
		{{0.f, 1.f}, "front"},
		{{1.f, 1.f}, "front_r"},
		{{-1.f, 1.f}, "front_l"},
		{{1.f, -1.f}, "bwd_r"},
		{{-1.f, -1.f}, "bwd_l"},
		{{0, -1.f}, "bwd"},
		{{1.f, 0.f}, "right"},
		{{-1.f, 0.f}, "left"}
	};
}

UPS_DodgeGameplayAbility::UPS_DodgeGameplayAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UPS_DodgeGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	APS_Character* APSCharacter = Cast<APS_Character>(ActorInfo->AvatarActor);
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

FName UPS_DodgeGameplayAbility::GetDodgeSectionName(const APS_Character* APSCharacter)
{
	if (!APSCharacter)
		return NAME_None;

	const FVector2d NormalizedMovement = UKismetMathLibrary::ToRounded2D(APSCharacter->AuxMovementVector);
	// If we're not moving, we don't want to dodge.
	if (UKismetMathLibrary::IsNearlyZero2D(NormalizedMovement))
		return NAME_None;

	const TMap<FVector2d, FName> Dodges = UPS_DodgeGameplayAbility_Const::Dodges.FilterByPredicate([&NormalizedMovement](const TPair<FVector2d, FName>& Dodge)
	{
		const FVector2d& DodgeVector = Dodge.Key;
		const float Length = UKismetMathLibrary::Subtract_Vector2DVector2D(NormalizedMovement, DodgeVector).Length(); 
		return UKismetMathLibrary::Less_DoubleDouble(Length, 1.f);
	});

	return Dodges.Num() > 0 ? Dodges.begin().Value() : NAME_None;
}

void UPS_DodgeGameplayAbility::OnAnimationCanStop(APS_Character* APSCharacter)
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
