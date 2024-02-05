#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "PS_PlayerCameraComponent.generated.h"

UCLASS()
class UPS_PlayerCameraComponent : public UCameraComponent
{
	GENERATED_BODY()

public:
	FSimpleMulticastDelegate OnLockTarget;

	void SetLockTarget(AActor* NewLockTarget);
	bool IsLockingTarget() const;
	
private:
	UPROPERTY(Transient)
		AActor* LockTarget = nullptr;

protected:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};

