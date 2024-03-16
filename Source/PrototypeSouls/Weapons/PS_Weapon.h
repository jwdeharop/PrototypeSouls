#pragma once

#include "PS_Types.h"
#include "GameFramework/Actor.h"
#include "PS_Weapon.generated.h"

class USphereComponent;
enum class EPS_WeaponType : uint8;
class UStaticMeshComponent;

USTRUCT()
struct FPS_ComboHit
{
	GENERATED_BODY()

	UPROPERTY(Transient)
		TWeakObjectPtr<AActor> HitActor;
	UPROPERTY(Transient)
		TArray<bool> HitInCombo;
};

UCLASS()
class APS_Weapon : public AActor
{
	GENERATED_BODY()

public:
	APS_Weapon();

	const TArray<FName>& GetWeaponSockets() const;
	EPS_WeaponType GetType() const;
	uint8 CurrentComboIndex = 1;

	uint8 ProcessCombo();
	void ResetCombo();
	void ActivateDamageArea(const bool bActivate);

protected:
	UPROPERTY(EditDefaultsOnly, Category = Weapon)
		EPS_WeaponType WeaponType = EPS_WeaponType::None;
	UPROPERTY(EditDefaultsOnly, Category = Weapon)
		UStaticMeshComponent* StaticMeshComponent = nullptr;
	UPROPERTY(EditDefaultsOnly, Category = Weapon)
		USphereComponent* SphereComponent = nullptr;
	UPROPERTY(EditDefaultsOnly, Category = Weapon)
		TArray<FName> WeaponSockets; // In case of various sockets, we will spawn an APS_Weapon for each socket. Ej. Double Axe is going to have 2 sockets.
	UPROPERTY(EditDefaultsOnly, Category = Weapon)
		uint8 MaxCombos = 3;
	UPROPERTY(EditDefaultsOnly, Category = "Weapon | Damage")
		float Damage = 0.f;

	UFUNCTION()
		void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual void BeginPlay() override;

private:
	UPROPERTY(Transient)
		TArray<FPS_ComboHit> HitInformation;
	UFUNCTION(Server, Reliable)
		void Server_ManageDamage(AActor* ActorToDamage);
};
