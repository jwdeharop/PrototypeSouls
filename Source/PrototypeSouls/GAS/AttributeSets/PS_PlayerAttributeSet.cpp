#include "GAS/AttributeSets/PS_PlayerAttributeSet.h"

#include "UnrealNetwork.h"
#include "Characters/PS_Character.h"
#include "GameFramework/CharacterMovementComponent.h"

void UPS_PlayerAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION_NOTIFY(UPS_PlayerAttributeSet, CurrentSpeed, COND_None, REPNOTIFY_OnChanged);
	DOREPLIFETIME_CONDITION_NOTIFY(UPS_PlayerAttributeSet, CurrentHealth, COND_None, REPNOTIFY_OnChanged);
	DOREPLIFETIME_CONDITION_NOTIFY(UPS_PlayerAttributeSet, MaxWalkSpeed, COND_None, REPNOTIFY_OnChanged);
}

void UPS_PlayerAttributeSet::OnRep_CurrentSpeed(const FGameplayAttributeData& OldMoveSpeed) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPS_PlayerAttributeSet, CurrentSpeed, OldMoveSpeed);
	const APS_Character* APSCharacter = Cast<APS_Character>(GetOwningActor());
	if (UCharacterMovementComponent* CharacterMovementComponent = APSCharacter ? Cast<UCharacterMovementComponent>(APSCharacter->GetMovementComponent()) : nullptr)
	{
		CharacterMovementComponent->MaxWalkSpeed = CurrentSpeed.GetCurrentValue();
	}
}

void UPS_PlayerAttributeSet::OnRep_CurrentHealth(const FGameplayAttributeData& OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPS_PlayerAttributeSet, CurrentHealth, OldHealth);
}

void UPS_PlayerAttributeSet::OnRep_MaxWalkSpeed(const FGameplayAttributeData& OlMaxWalkSpeed) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPS_PlayerAttributeSet, MaxWalkSpeed, OlMaxWalkSpeed);
}
