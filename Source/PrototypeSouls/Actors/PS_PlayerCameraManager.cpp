#include "Actors/PS_PlayerCameraManager.h"
#include "Characters/Player/PS_PlayerCharacter.h"
#include "Components/PS_PlayerCameraComponent.h"

void APS_PlayerCameraManager::LockTarget(APS_PlayerCharacter* Character, TArray<FHitResult>& Hits)
{
	UPS_PlayerCameraComponent* Camera = GetCameraComponent();

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
		return Dot > 0.6f ;
	});

	SortHitsByDistance(Character, Hits);
	OnTargetLocked.Broadcast(Hits.IsEmpty() ? nullptr : Hits[0].GetActor());
}

void APS_PlayerCameraManager::TryChangeTarget(APS_PlayerCharacter* Character, TArray<FHitResult>& Hits, const FVector& NormalizedInput)
{
	UPS_PlayerCameraComponent* Camera = GetCameraComponent();
	if (!Camera)
		return;

	Hits = Hits.FilterByPredicate([Character, Camera, NormalizedInput](const FHitResult& Hit)
	{
		const float Dot = FVector::DotProduct((Camera->GetRightVector()).GetSafeNormal(), (Hit.GetActor()->GetActorLocation() - Character->GetActorLocation()).GetSafeNormal()); 
		return (NormalizedInput.X < 0.f && Dot < -0.04f) || (NormalizedInput.X > 0.f && Dot > 0.04f);
	});

	SortHitsByDistance(Character, Hits);
	if (!Hits.IsEmpty())
	{
		OnTargetLocked.Broadcast(Hits[0].GetActor());
	}
}

UPS_PlayerCameraComponent* APS_PlayerCameraManager::GetCameraComponent() const
{
	const APlayerController* Controller = GetOwningPlayerController();
	const APS_PlayerCharacter* PlayerCharacter = Controller ? Controller->GetPawn<APS_PlayerCharacter>() : nullptr;
	return PlayerCharacter ? PlayerCharacter->GetFollowCamera() : nullptr;
}

void APS_PlayerCameraManager::SortHitsByDistance(APS_PlayerCharacter* Character, TArray<FHitResult>& Hits)
{
	Hits.Sort([Character](const FHitResult& HitResult1, const FHitResult& HitResult2)
{
	const float FirstDistance = Character->GetDistanceTo(HitResult1.GetActor());
	const float SecondDistance = Character->GetDistanceTo(HitResult2.GetActor());
	return FirstDistance < SecondDistance;
});
}
