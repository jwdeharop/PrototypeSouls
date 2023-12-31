#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PS_PlayerController.generated.h"

UCLASS(config=Game)
class APS_PlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void AcknowledgePossession(APawn* P) override;
};

