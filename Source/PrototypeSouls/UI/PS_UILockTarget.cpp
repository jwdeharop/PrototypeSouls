#include "UI/PS_UILockTarget.h"
#include "Actors/PS_PlayerCameraManager.h"
#include "Kismet/GameplayStatics.h"

void UPS_UILockTarget::SetOwner(AActor* WidgetOwner)
{
	Owner = WidgetOwner;
}

void UPS_UILockTarget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	// When initializing we will bind to the camera manager. Widgets only exists locally, so player index will always be 0.
	if (APS_PlayerCameraManager* CameraManager = Cast<APS_PlayerCameraManager>(UGameplayStatics::GetPlayerCameraManager(this, 0)))
	{
		CameraManager->OnTargetLocked.AddUniqueDynamic(this, &UPS_UILockTarget::TargetLocked);
	}
	
	TargetLocked(nullptr);
}

void UPS_UILockTarget::TargetLocked(const AActor* LockedTarget)
{
	const bool bOwnerIsValid = Owner != nullptr;
	const bool bIsSameTarget = Owner == LockedTarget;
	SetVisibility(bOwnerIsValid && bIsSameTarget ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
}
