#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PS_PlayerController.generated.h"

class APS_PlayerState;
DECLARE_MULTICAST_DELEGATE_OneParam(FOnControllerGetsPlayerState, APS_PlayerState*)
DECLARE_MULTICAST_DELEGATE_OneParam(FTryChangeTarget, const FVector&)

UCLASS(config=Game)
class APS_PlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	FOnControllerGetsPlayerState OnControllerGetsPlayerState;
	FTryChangeTarget OnTryChangeTarget;

	void TryChangeCameraTarget(const FVector& NormalizedInput);

protected:
	virtual void OnPossess(APawn* P) override;
	virtual void PostProcessInput(const float DeltaTime, const bool bGamePaused) override;
	virtual void OnRep_PlayerState() override;
	virtual void BeginPlay() override;

private:
	UFUNCTION()
		void OnTargetLocked(AActor* LockedTarget);
};

