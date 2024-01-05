#pragma once

#include "PS_Types.h"
#include "GameFramework/Actor.h"
#include "PS_Weapon.generated.h"

enum class EPS_WeaponType : uint8;
class UStaticMeshComponent;

UCLASS()
class APS_Weapon : public AActor
{
	GENERATED_BODY()

public:
	APS_Weapon();

	const TArray<FName>& GetWeaponSockets() const;

protected:
	UPROPERTY(EditDefaultsOnly, Category = Weapon)
		EPS_WeaponType WeaponType = EPS_WeaponType::None;
	UPROPERTY(EditDefaultsOnly, Category = Weapon)
		UStaticMeshComponent* StaticMeshComponent = nullptr;
	UPROPERTY(EditDefaultsOnly, Category = Weapon)
		TArray<FName> WeaponSockets; // In case of various sockets, we will spawn an APS_Weapon for each socket. Ej. Double Axe is going to have 2 sockets.
	
};
