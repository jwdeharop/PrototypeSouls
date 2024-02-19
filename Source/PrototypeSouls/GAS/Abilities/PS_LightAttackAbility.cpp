#include "GAS/Abilities/PS_LightAttackAbility.h"

#include "AbilitySystemComponent.h"
#include "AbilityTask_WaitInputPress.h"
#include "Characters/Player/PS_PlayerCharacter.h"
#include "Components/PS_AbilitySystemComponent.h"
#include "DataAssets/PS_WeaponComboConfig.h"
#include "GAS/Tasks/PS_AbilityTask_PlayMontageAndWaitForEvent.h"
#include "Weapons/PS_Weapon.h"

void UPS_LightAttackAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!K2_CommitAbility())
	{
		K2_EndAbility();
		return;
	}

	// When trying to light attack, we check what's our current weapon.
	APSCharacter = CastChecked<APS_PlayerCharacter>(ActorInfo->AvatarActor);

	if (!APSCharacter)
	{
		K2_EndAbility();
		return;
	}

	if (!PlayCurrentCombo())
	{
		K2_EndAbility();
	}
}

void UPS_LightAttackAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (!APSCharacter)
	{
		Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
		return;
	}

	if (APS_Weapon* APSWeapon = APSCharacter->GetCurrentWeapon())
	{
		APSWeapon->ResetCombo();
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UPS_LightAttackAbility::OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData)
{
	K2_EndAbility();
}

void UPS_LightAttackAbility::OnPressed(float TimeWaited)
{
	BindToPressEvent();

	if (APSCharacter && APSCharacter->bCanChangeCombo)
	{
		PlayCurrentCombo();
		APSCharacter->bCanChangeCombo = false;
	}
}

void UPS_LightAttackAbility::OnEventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{
	const APS_PlayerCharacter* Character = Cast<APS_PlayerCharacter>(GetActorInfo().AvatarActor);
	UPS_AbilitySystemComponent* AbilitySystemComponent = Character ? Cast<UPS_AbilitySystemComponent>(Character->GetAbilitySystemComponent()) : nullptr;
	const bool bCanCheckTag = (EventTag == FGameplayTag::RequestGameplayTag("Ability.LightAttack.StartDamage")) && AbilitySystemComponent; 
	if (!bCanCheckTag)
		return;

	const bool bHasTag = AbilitySystemComponent->HasMatchingGameplayTag(EventTag);
	if (bHasTag)
	{
		AbilitySystemComponent->RemoveLooseGameplayTag(EventTag);
	}
	else
	{
		AbilitySystemComponent->AddLooseGameplayTag(EventTag);
	}
}

bool UPS_LightAttackAbility::PlayCurrentCombo()
{
	BindToPressEvent();

	APS_Weapon* APSWeapon = APSCharacter ? APSCharacter->GetCurrentWeapon() : nullptr;
	if (!APSWeapon)
		return false;

	UAnimInstance* AnimInstance = APSCharacter->GetMesh() ? APSCharacter->GetMesh()->GetAnimInstance() : nullptr;

	if (AnimInstance)
	{
		AnimInstance->StopAllMontages(1.f);
	}

	FPS_ComboWeaponInfo ComboWeaponInfo = APSCharacter->GetCurrentWeaponComboInfo();
	UAnimMontage* CurrentCombo = ComboWeaponInfo.ComboMontages[EPS_ComboType::LightCombo];
	if (!CurrentCombo)
	{
		return false;
	}

	const FString ComboSection = ComboWeaponInfo.ComboSections[EPS_ComboType::LightCombo].ToString() + FString::FromInt(APSWeapon->ProcessCombo());
	const FGameplayTagContainer TagEvents;
	if (UPS_AbilityTask_PlayMontageAndWaitForEvent* PlayMontageAndWaitTask = UPS_AbilityTask_PlayMontageAndWaitForEvent::PlayMontageAndWaitForEvent(this, NAME_None, CurrentCombo, TagEvents, 1.f, *ComboSection))
	{
		PlayMontageAndWaitTask->OnCompleted.AddUniqueDynamic(this, &ThisClass::OnCancelled);
		PlayMontageAndWaitTask->OnEventReceived.AddUniqueDynamic(this, &UPS_LightAttackAbility::OnEventReceived);
		PlayMontageAndWaitTask->ReadyForActivation();
	}

	return true;
}

void UPS_LightAttackAbility::BindToPressEvent()
{
	if (UAbilityTask_WaitInputPress* InputPressTask = UAbilityTask_WaitInputPress::WaitInputPress(this))
	{
		// We bind to input pressed to check combos.
		InputPressTask->OnPress.AddUniqueDynamic(this, &UPS_LightAttackAbility::OnPressed);
		InputPressTask->ReadyForActivation();
	}
}