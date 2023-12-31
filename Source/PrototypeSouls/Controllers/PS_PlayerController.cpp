#include "Controllers/PS_PlayerController.h"
#include "AbilitySystemComponent.h"
#include "Characters/PS_Character.h"

void APS_PlayerController::AcknowledgePossession(APawn* P)
{
	Super::AcknowledgePossession(P);
	if (APS_Character* APSCharacter = Cast<APS_Character>(P))
	{
		APSCharacter->GetAbilitySystemComponent()->InitAbilityActorInfo(APSCharacter, APSCharacter);
	}
}
