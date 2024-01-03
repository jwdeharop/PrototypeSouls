#include "Controllers/PS_PlayerController.h"
#include "AbilitySystemComponent.h"
#include "Characters/PS_Character.h"
#include "Components/PS_AbilitySystemComponent.h"

void APS_PlayerController::AcknowledgePossession(APawn* P)
{
	Super::AcknowledgePossession(P);
	if (APS_Character* APSCharacter = Cast<APS_Character>(P))
	{
		APSCharacter->GetAbilitySystemComponent()->InitAbilityActorInfo(APSCharacter, APSCharacter);
	}
}

void APS_PlayerController::PostProcessInput(const float DeltaTime, const bool bGamePaused)
{
	Super::PostProcessInput(DeltaTime, bGamePaused);

	const APS_Character* APSCharacter = Cast<APS_Character>(GetPawn());
	if (UPS_AbilitySystemComponent* AbilitySystemComponent = APSCharacter ? Cast<UPS_AbilitySystemComponent>(APSCharacter->GetAbilitySystemComponent()) : nullptr)
	{
		AbilitySystemComponent->ProcessAbilityInput(DeltaTime, bGamePaused);
	}
}
