#include "Components/PS_MovementComponent.h"
#include "Characters/PS_Character.h"

float UPS_MovementComponent::GetMaxSpeed() const
{
	const APS_Character* APSCharacter = Cast<APS_Character>(GetOwner());
	return APSCharacter ? APSCharacter->GetMaxSpeed() : Super::GetMaxSpeed();
}
