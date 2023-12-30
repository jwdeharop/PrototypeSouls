#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "PS_AssetManager.generated.h"

UCLASS()
class UPS_AssetManager : public UAssetManager
{
	GENERATED_BODY()

public:

	static UPS_AssetManager& Get();

	virtual void StartInitialLoading() override;
};
