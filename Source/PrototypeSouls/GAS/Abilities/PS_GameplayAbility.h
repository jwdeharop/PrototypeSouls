#pragma once

#include "PS_Types.h"
#include "GameplayAbility.h"
#include "PS_GameplayAbility.generated.h"

UCLASS()
class UPS_GameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
		EPS_AbilityInputID InputID = EPS_AbilityInputID::None;
	UPROPERTY(EditDefaultsOnly)
		FGameplayTag InputTag;
};