#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EPS_AbilityInputID : uint8
{
	None,
	Confirm,
	Cancel,
	Sprint,
	Dodge,
	LightAttack
};

UENUM(BlueprintType)
enum class EPS_WeaponType : uint8
{
	None = 0,
	Axe
};