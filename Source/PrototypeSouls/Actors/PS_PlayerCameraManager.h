#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "PS_PlayerCameraManager.generated.h"

class APS_PlayerCharacter;
class UPS_PlayerCameraComponent;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTargetLocked, const AActor*, LockedTarget);

UCLASS()
class APS_PlayerCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()

public:
	FTargetLocked OnTargetLocked;

	void LockTarget(APS_PlayerCharacter* Character, TArray<FHitResult>& Hits) const;
	void TryChangeTarget(APS_PlayerCharacter* Character, TArray<FHitResult>& Hits, const FVector& NormalizedInput) const;

	UPS_PlayerCameraComponent* GetCameraComponent() const;

private:
	static void SortHitsByDistance(APS_PlayerCharacter* Character, TArray<FHitResult>& Hits);
};
