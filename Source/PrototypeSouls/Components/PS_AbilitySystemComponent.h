#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "PS_AbilitySystemComponent.generated.h"

UCLASS()
class UPS_AbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	bool bAbilitiesGranted = false;
};

