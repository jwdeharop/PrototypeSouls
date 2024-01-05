#include "Weapons/PS_Weapon.h"

APS_Weapon::APS_Weapon()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponnet"));
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(RootComponent);

	SetReplicates(true);
}

const TArray<FName>& APS_Weapon::GetWeaponSockets() const
{
	return WeaponSockets;
}
