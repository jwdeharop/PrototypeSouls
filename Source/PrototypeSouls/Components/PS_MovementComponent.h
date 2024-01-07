#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PS_MovementComponent.generated.h"

UCLASS()
class UPS_MovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

protected:
	virtual float GetMaxSpeed() const override;
};

