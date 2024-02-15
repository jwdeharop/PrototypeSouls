#include "GAS/Abilities/PS_LockTargetAbility.h"

#include "Actors/PS_PlayerCameraManager.h"
#include "Characters/Player/PS_PlayerCharacter.h"
#include "Components/PS_PlayerCameraComponent.h"
#include "Controllers/PS_PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Libraries/PS_NetLibrary.h"

UPS_LockTargetAbility::UPS_LockTargetAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UPS_LockTargetAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	APS_PlayerCharacter* Character = ActorInfo ? Cast<APS_PlayerCharacter>(ActorInfo->AvatarActor) : nullptr;

	if (!Character)
	{
		K2_EndAbility();
		return;
	}

	if (UPS_NetLibrary::IsClient(Character) && Character->IsLocallyControlled())
	{
		const UPS_PlayerCameraComponent* Camera = Character->GetFollowCamera();
		if (!Camera)
		{
			K2_EndAbility();
			return;
		}

		APS_PlayerCameraManager* CameraManager = Cast<APS_PlayerCameraManager>(UGameplayStatics::GetPlayerCameraManager(this, 0));
		if (!CameraManager)
		{
			K2_EndAbility();
			return;
		}

		if (Camera->IsLockingTarget())
		{
			TArray<FHitResult> Hits;
			CameraManager->LockTarget(Character, Hits);
			K2_EndAbility();
			return;
		}

		const FVector& StartLocation = Character->GetActorLocation();
		TArray<FHitResult> Hits;

		if (UKismetSystemLibrary::SphereTraceMultiForObjects(Character, StartLocation, StartLocation, Radius, { EObjectTypeQuery::ObjectTypeQuery7}, false, {ActorInfo->AvatarActor.Get()}, EDrawDebugTrace::None, Hits, true))
		{
			// We let the manager filter the data. This happens only locally so we know that the player index will always be 0.
			CameraManager->LockTarget(Character, Hits);
			if (APS_PlayerController* Controller = Cast<APS_PlayerController>(Character->GetController()))
			{
				Controller->OnTryChangeTarget.AddUObject(this, &UPS_LockTargetAbility::TryChangeTarget);
			}
		}
	}

	K2_EndAbility();
}

void UPS_LockTargetAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	const APS_PlayerCharacter* Character = ActorInfo ? Cast<APS_PlayerCharacter>(ActorInfo->AvatarActor) : nullptr;
	const UPS_PlayerCameraComponent* Camera = Character ? Character->GetFollowCamera() : nullptr;
	APS_PlayerController* Controller = Character ? Cast<APS_PlayerController>(Character->GetController()) : nullptr;
	if (Controller && Camera && !Camera->IsLockingTarget())
	{
		Controller->OnTryChangeTarget.RemoveAll(this);
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UPS_LockTargetAbility::TryChangeTarget(const FVector& Normalized) const
{
	APS_PlayerCharacter* Character = Cast<APS_PlayerCharacter>(GetActorInfo().AvatarActor);
	const UPS_PlayerCameraComponent* Camera = Character ? Character->GetFollowCamera() : nullptr;
	if (!Camera)
		return;
	
	const FVector& StartLocation = Character->GetActorLocation();
	TArray<FHitResult> Hits;
	if (UKismetSystemLibrary::SphereTraceMultiForObjects(Character, StartLocation, StartLocation, Radius, { EObjectTypeQuery::ObjectTypeQuery7}, false, {Character}, EDrawDebugTrace::None, Hits, true))
	{
		Hits = Hits.FilterByPredicate([Camera](const FHitResult& Result)
		{
			return Result.GetActor() != Camera->GetActorLocked();
		});

		if (APS_PlayerCameraManager* CameraManager = Cast<APS_PlayerCameraManager>(UGameplayStatics::GetPlayerCameraManager(this, 0)))
		{
			CameraManager->TryChangeTarget(Character, Hits, Normalized);
		}
	}
}
