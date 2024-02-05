#include "Components/PS_PlayerCameraComponent.h"

#include "Characters/Player/PS_PlayerCharacter.h"
#include "Kismet/KismetMathLibrary.h"

void UPS_PlayerCameraComponent::SetLockTarget(AActor* NewLockTarget)
{
	LockTarget = NewLockTarget;
	OnLockTarget.Broadcast();
}

bool UPS_PlayerCameraComponent::IsLockingTarget() const
{
	return LockTarget != nullptr;
}

void UPS_PlayerCameraComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (LockTarget)
	{
		const APS_PlayerCharacter* Character = Cast<APS_PlayerCharacter>(GetOwner());
		APlayerController* PlayerController = Character ? Cast<APlayerController>(Character->GetController()) : nullptr;
		if (PlayerController)
		{
			PlayerController->SetControlRotation(UKismetMathLibrary::FindLookAtRotation(Character->GetActorLocation(), LockTarget->GetActorLocation()));
		}
	}
}
