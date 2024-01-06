#pragma once

#include "PS_Types.h"
#include "Engine/DataAsset.h"
#include "PS_WeaponComboConfig.generated.h"

USTRUCT(BlueprintType)
struct FPS_ComboWeaponInfo
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly)
		EPS_WeaponType Type = EPS_WeaponType::None;
	UPROPERTY(EditDefaultsOnly)
		TMap<EPS_ComboType, FName> ComboSections;
	UPROPERTY(EditDefaultsOnly)
		TMap<EPS_ComboType, UAnimMontage*> ComboMontages;
};

UCLASS()
class UPS_WeaponComboConfig : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
		TArray<FPS_ComboWeaponInfo> WeaponConfig;
};