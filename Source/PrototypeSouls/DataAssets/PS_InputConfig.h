#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "PS_InputConfig.generated.h"

class UInputAction;

USTRUCT(BlueprintType)
struct FPS_InputConfig
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
		FGameplayTag AbilityTag;
	UPROPERTY(EditDefaultsOnly)
		UInputAction* InputAction = nullptr;
};

UCLASS()
class UPS_InputConfigDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
		TArray<FPS_InputConfig> InputConfig;
};