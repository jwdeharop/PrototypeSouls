#include "GAS/Abilities/PS_LockTargetAbility.h"
#include "Characters/Player/PS_PlayerCharacter.h"
#include "Components/PS_PlayerCameraComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Libraries/PS_NetLibrary.h"

namespace UPS_LockTargetAbility_Consts
{
	static constexpr float CameraThresholdDot = 0.6f;
}

UPS_LockTargetAbility::UPS_LockTargetAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UPS_LockTargetAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	const APS_PlayerCharacter* Character = ActorInfo ? Cast<APS_PlayerCharacter>(ActorInfo->AvatarActor) : nullptr;

	if (!Character)
	{
		K2_EndAbility();
		return;
	}

	if (UPS_NetLibrary::IsClient(Character))
	{
		UPS_PlayerCameraComponent* Camera = Character->GetFollowCamera();
		if (!Camera)
		{
			K2_EndAbility();
			return;
		}

		if (Camera->IsLockingTarget())
		{
			Camera->SetLockTarget(nullptr);
			APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
			if (PlayerController)
			{
				PlayerController->SetIgnoreLookInput(false);
			}
			K2_EndAbility();
			return;
		}

		const FVector& StartLocation = Character->GetActorLocation();
		TArray<FHitResult> Hits;

		if (UKismetSystemLibrary::SphereTraceMultiForObjects(Character, StartLocation, StartLocation, Radius, { EObjectTypeQuery::ObjectTypeQuery7}, false, {ActorInfo->AvatarActor.Get()}, EDrawDebugTrace::None, Hits, true))
		{
			// We only want the hits that are in front of the camera. Dot product time!
			Hits = Hits.FilterByPredicate([Camera](const FHitResult& HitResult)
			{
				const AActor* OtherActor = HitResult.GetActor();
				if (!OtherActor)
					return false;

				const FVector& Dir = Camera->GetForwardVector();
				FVector Offset = OtherActor->GetActorLocation() - Camera->GetComponentLocation();
				Offset = Offset.GetSafeNormal();
				const float Dot = FVector::DotProduct(Dir, Offset);
				return Dot > UPS_LockTargetAbility_Consts::CameraThresholdDot ;
			});

			Hits.Sort([Character](const FHitResult& HitResult1, const FHitResult& HitResult2)
			{
				const float FirstDistance = Character->GetDistanceTo(HitResult1.GetActor());
				const float SecondDistance = Character->GetDistanceTo(HitResult2.GetActor());
				return FirstDistance < SecondDistance;
			});

			if (!Hits.IsEmpty())
			{
			    // This could be done with Observer pattern (maybe). But I dont have time to do it.
				Camera->SetLockTarget(Hits[0].GetActor());
				APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
				if (PlayerController)
				{
					PlayerController->SetIgnoreLookInput(true);
				}
			}
		}
	}

	K2_EndAbility();
}