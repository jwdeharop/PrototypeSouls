#include "Weapons/PS_Weapon.h"

APS_Weapon::APS_Weapon()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponnet"));
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(RootComponent);

	bReplicates = true;
}

const TArray<FName>& APS_Weapon::GetWeaponSockets() const
{
	return WeaponSockets;
}

EPS_WeaponType APS_Weapon::GetType() const
{
	return WeaponType;
}

uint8 APS_Weapon::ProcessCombo()
{
	if (CurrentComboIndex > MaxCombos)
	{
		ResetCombo();
	}

	return CurrentComboIndex++;
}

void APS_Weapon::ResetCombo()
{
	CurrentComboIndex = 1;
}
