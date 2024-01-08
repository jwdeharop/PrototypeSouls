#include "Controllers/PS_PlayerController.h"
#include "AbilitySystemComponent.h"
#include "Characters/PS_PlayerCharacter.h"
#include "Characters/PS_PlayerState.h"
#include "Components/PS_AbilitySystemComponent.h"

void APS_PlayerController::OnPossess(APawn* P)
{
	Super::OnPossess(P);

	if (APS_PlayerState* APSPlayerState = GetPlayerState<APS_PlayerState>())
	{
		APSPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(APSPlayerState, P);
	}
}

void APS_PlayerController::PostProcessInput(const float DeltaTime, const bool bGamePaused)
{
	Super::PostProcessInput(DeltaTime, bGamePaused);

	const APS_PlayerCharacter* APSCharacter = Cast<APS_PlayerCharacter>(GetPawn());
	if (UPS_AbilitySystemComponent* AbilitySystemComponent = APSCharacter ? Cast<UPS_AbilitySystemComponent>(APSCharacter->GetAbilitySystemComponent()) : nullptr)
	{
		AbilitySystemComponent->ProcessAbilityInput(DeltaTime, bGamePaused);
	}
}

void APS_PlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	OnControllerGetsPlayerState.Broadcast(GetPlayerState<APS_PlayerState>());
}
