#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "PS_BaseAttributeSet.h"
#include "PS_DamageExecCalculations.generated.h"

struct FPS_DamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);

	FPS_DamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UPS_BaseAttributeSet, Armor, Target, false);
	}
};

static const FPS_DamageStatics& DamageStatics()
{
	static FPS_DamageStatics DStatics;
	return DStatics;
}

UCLASS()
class UPS_DamageExecCalculations : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
	UPS_DamageExecCalculations();

protected:
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};