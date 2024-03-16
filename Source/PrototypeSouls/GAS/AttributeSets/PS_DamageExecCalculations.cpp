#include "GAS/AttributeSets/PS_DamageExecCalculations.h"

UPS_DamageExecCalculations::UPS_DamageExecCalculations()
{
	RelevantAttributesToCapture.Add(DamageStatics().ArmorDef);
}

void UPS_DamageExecCalculations::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	Super::Execute_Implementation(ExecutionParams, OutExecutionOutput);
}
