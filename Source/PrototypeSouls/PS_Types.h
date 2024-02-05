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
	LightAttack,
	LockTarget
};

UENUM(BlueprintType)
enum class EPS_WeaponType : uint8
{
	None = 0,
	Axe
};

UENUM(BlueprintType)
enum class EPS_ComboType : uint8
{
	LightCombo = 0
};

UENUM()
enum class EPS_AnimationDirection : uint8
{
	PositiveBack = 0,
	NegativeBack,
	BackLeft,
	BackRight,
	Left,
	Right,
	Front,
	FrontLeft,
	FrontRight
};