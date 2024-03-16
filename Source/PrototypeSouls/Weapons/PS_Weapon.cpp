#include "Weapons/PS_Weapon.h"
#include "Characters/Player/PS_PlayerCharacter.h"
#include "Components/SphereComponent.h"

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
	// When the combo is reset, we reset the hit actors as well.
	HitInformation.Empty();
}

void APS_Weapon::ActivateDamageArea(const bool bActivate)
{
	if (!SphereComponent)
		return;

	SphereComponent->SetActive(bActivate);
	if (bActivate)
	{
		SphereComponent->OnComponentBeginOverlap.AddUniqueDynamic(this, &APS_Weapon::OnComponentBeginOverlap);
	}
	else
	{
		SphereComponent->OnComponentBeginOverlap.RemoveDynamic(this, &APS_Weapon::OnComponentBeginOverlap);
	}
}

void APS_Weapon::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	const FPS_ComboHit* ComboHit = HitInformation.FindByPredicate([OtherActor](const FPS_ComboHit& ComboHit)
	{
		return ComboHit.HitActor == OtherActor;		
	});

	// At this point, CurrentComboIndex has already been processed.
	if (ComboHit && ComboHit->HitInCombo[CurrentComboIndex - 2])
		return;

	if (!ComboHit)
	{
		FPS_ComboHit ComboHitInfo;
		ComboHitInfo.HitActor = OtherActor;
		ComboHitInfo.HitInCombo.Init(false, MaxCombos);
		ComboHitInfo.HitInCombo[CurrentComboIndex - 2] = true;
		HitInformation.Add(ComboHitInfo);
	}

	// When detecting a hit, we will inform the server.
	// All hit management should be managed by the Damageable Component.
	Server_ManageDamage(OtherActor);
}

void APS_Weapon::BeginPlay()
{
	Super::BeginPlay();

	const APS_CharacterBase* CharacterBase = Cast<APS_CharacterBase>(GetOwner());
	if (CharacterBase && CharacterBase->IsLocallyControlled())
	{
		// Only the local character will manage damage area effects and will inform the server to do the appropriate thing. 
		constexpr bool bActivate = false;
		ActivateDamageArea(bActivate);
	}
	else
	{
		SphereComponent->DestroyComponent();
		SphereComponent = nullptr;
	}
}

void APS_Weapon::Server_ManageDamage_Implementation(AActor* ActorToDamage)
{
	if (!ActorToDamage)
	{
		UE_LOG(LogTemp, Error, TEXT("APS_Weapon::Server_ManageDamage_Implementation trying to damage an invalid actor."));
		return;
	}
	
	IPS_DamageableInterface* DamageableInterface = Cast<IPS_DamageableInterface>(ActorToDamage);
	const UPS_DamageableComponent* DamageableComponent = DamageableInterface ? DamageableInterface->GetDamageableComponent() : nullptr;
	if (!DamageableComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("APS_Weapon::Server_ManageDamage_Implementation damaged actor %s doesn't have a damageable component."), *ActorToDamage->GetName());
		return;
	}
}
