#pragma once

#include "CoreMinimal.h"
#include "UserWidget.h"
#include "PS_UILockTarget.generated.h"

UCLASS()
class UPS_UILockTarget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetOwner(AActor* WidgetOwner);

protected:
	UPROPERTY(Transient)
		TWeakObjectPtr<AActor> Owner = nullptr;

	virtual void NativeOnInitialized() override;

private:
	UFUNCTION()
		void TargetLocked(const AActor* LockedTarget);
};

