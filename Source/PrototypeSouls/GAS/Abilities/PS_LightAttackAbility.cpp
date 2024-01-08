#include "GAS/Abilities/PS_LightAttackAbility.h"
#include "AbilityTask_PlayMontageAndWait.h"
#include "AbilityTask_WaitInputPress.h"
#include "Characters/PS_PlayerCharacter.h"
#include "DataAssets/PS_WeaponComboConfig.h"
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

	APSCharacter->bUseControllerRotationYaw = true;

	if (!PlayCurrentCombo())
	{
		K2_EndAbility();
	}
}

void UPS_LightAttackAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (APS_Weapon* APSWeapon = APSCharacter ? APSCharacter->GetCurrentWeapon() : nullptr)
	{
		APSWeapon->ResetCombo();
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UPS_LightAttackAbility::OnCancelled()
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
	if (UAbilityTask_PlayMontageAndWait* PlayMontageAndWaitTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, CurrentCombo, 1.f, FName(*ComboSection)))
	{
		PlayMontageAndWaitTask->OnCompleted.AddUniqueDynamic(this, &ThisClass::OnCancelled);
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

void UPS_LightAttackAbility::OnCanChangeYaw()
{
	APSCharacter->bUseControllerRotationYaw = false;
}
