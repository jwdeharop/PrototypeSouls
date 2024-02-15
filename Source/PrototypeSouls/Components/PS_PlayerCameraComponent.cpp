#include "Components/PS_PlayerCameraComponent.h"
#include "Actors/PS_PlayerCameraManager.h"
#include "Characters/Player/PS_PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

bool UPS_PlayerCameraComponent::IsLockingTarget() const
{
	return LockTarget != nullptr;
}

const AActor* UPS_PlayerCameraComponent::GetActorLocked() const
{
	return LockTarget;
}

void UPS_PlayerCameraComponent::OnTargetLocked(const AActor* LockedTarget)
{
	LockTarget = LockedTarget;
}

void UPS_PlayerCameraComponent::BeginPlay()
{
	Super::BeginPlay();

	if (APS_PlayerCameraManager* CameraManager = Cast<APS_PlayerCameraManager>(UGameplayStatics::GetPlayerCameraManager(this, 0)))
	{
		CameraManager->OnTargetLocked.AddUniqueDynamic(this, &UPS_PlayerCameraComponent::OnTargetLocked);
	}
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
			PlayerController->SetControlRotation(UKismetMathLibrary::RLerp(GetComponentRotation(), UKismetMathLibrary::FindLookAtRotation(Character->GetActorLocation(), LockTarget->GetActorLocation()), 0.1f, true));
		}
	}
}
