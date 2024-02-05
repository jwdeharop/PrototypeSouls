#include "Weapons/PS_Weapon.h"

#include "Components/SphereComponent.h"
#include "Libraries/PS_NetLibrary.h"

APS_Weapon::APS_Weapon()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponnet"));
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(RootComponent);

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetupAttachment(StaticMeshComponent);

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

void APS_Weapon::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
}

void APS_Weapon::BeginPlay()
{
	Super::BeginPlay();

	if (UPS_NetLibrary::IsServer(this))
	{
		// Server will manage all the overlaps of the weapon.
		SphereComponent->OnComponentBeginOverlap.AddUniqueDynamic(this, &APS_Weapon::OnComponentBeginOverlap);
		SphereComponent->SetActive(false);
	}
	else
	{
		SphereComponent->DestroyComponent();
		SphereComponent = nullptr;
	}
}
