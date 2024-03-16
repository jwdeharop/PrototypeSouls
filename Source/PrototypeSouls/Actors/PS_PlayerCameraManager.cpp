#include "Actors/PS_PlayerCameraManager.h"
#include "Characters/Player/PS_PlayerCharacter.h"
#include "Components/PS_PlayerCameraComponent.h"
#include "Kismet/GameplayStatics.h"

void APS_PlayerCameraManager::LockTarget(APS_PlayerCharacter* Character, TArray<FHitResult>& Hits)
{
	UPS_PlayerCameraComponent* Camera = GetCameraComponent();
	const APlayerCameraManager* CameraManager = UGameplayStatics::GetPlayerCameraManager(this, Cast<APlayerController>(Character->GetController())->NetPlayerIndex);
	if (!CameraManager || !Camera)
		return;

	const float FOVAngle = CameraManager->GetFOVAngle();

	// We only want the hits that are in front of the camera. Dot product time!
	Hits = Hits.FilterByPredicate([Camera, FOVAngle](const FHitResult& HitResult)
	{
		const AActor* OtherActor = HitResult.GetActor();
		if (!OtherActor)
			return false;

		const FVector DirectionToActor = (OtherActor->GetActorLocation() - Camera->GetComponentLocation()).GetSafeNormal();
		const FVector& CameraForwardVector = Camera->GetComponentRotation().Vector();
		const float DotProduct = FVector::DotProduct(CameraForwardVector, DirectionToActor);
		const float CosFOV = FMath::Cos(FMath::DegreesToRadians(FOVAngle * 0.5f));
		const float SubTract = FMath::Abs(DotProduct - CosFOV);
		return SubTract > 0.015 && SubTract < 0.2;
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
