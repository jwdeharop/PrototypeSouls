#include "Components/PS_MovementComponent.h"
#include "Characters/PS_PlayerCharacter.h"

float UPS_MovementComponent::GetMaxSpeed() const
{
	const APS_PlayerCharacter* APSCharacter = Cast<APS_PlayerCharacter>(GetOwner());
	return APSCharacter ? APSCharacter->GetMaxSpeed() : Super::GetMaxSpeed();
}
