#pragma once

#include "CoreMinimal.h"
#include "PS_DamageableInterface.generated.h"

class UPS_DamageableComponent;

UINTERFACE(MinimalAPI, Blueprintable)
class UPS_DamageableInterface : public UInterface
{
	GENERATED_BODY()
};

class IPS_DamageableInterface
{
	GENERATED_BODY()

public:
	virtual UPS_DamageableComponent* GetDamageableComponent() { return nullptr; }
};
