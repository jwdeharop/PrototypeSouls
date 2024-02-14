#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "PS_PlayerCameraComponent.generated.h"

DECLARE_DELEGATE_OneParam(FTargetChanged, const FVector&)
UCLASS()
class UPS_PlayerCameraComponent : public UCameraComponent
{
	GENERATED_BODY()

public:
	FTargetChanged OnChangeTarget;

	void SetLockTarget(AActor* NewLockTarget);
	bool IsLockingTarget() const;
	AActor* GetActorLocked();
	void Prova(const FVector& Normalized);
	
private:
	UPROPERTY(Transient)
		AActor* LockTarget = nullptr;

protected:
	UFUNCTION()
		void OnTargetLocked(AActor* LockedTarget);

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};

