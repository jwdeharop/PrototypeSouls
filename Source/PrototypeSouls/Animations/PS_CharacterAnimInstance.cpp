#include "Animations/PS_CharacterAnimInstance.h" 
#include "Characters/PS_PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "KismetAnimationLibrary.h"
#include "PS_Types.h"

namespace UPS_CharacterAnimInstance_Consts
{
	static constexpr float SpeedMoveThreshold = 3.f;
}

float UPS_CharacterAnimInstance::GetDirection() const
{
	return Direction;
}

EPS_AnimationDirection UPS_CharacterAnimInstance::GetAnimationDirection()
{
	const float LocalDirection = Direction; 
	AnimationDirections.Sort([&LocalDirection](const FPS_AnimationDirection& AnimationDirectionA, const FPS_AnimationDirection& AnimationDirectionB)
	{
		const float SubA = UKismetMathLibrary::Abs(AnimationDirectionA.DirectionValue - LocalDirection);
		const float SubB = UKismetMathLibrary::Abs(AnimationDirectionB.DirectionValue - LocalDirection);
		return SubA < SubB;
	});

	return AnimationDirections[0].Direction;
}

void UPS_CharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	MyCharacter = Cast<APS_PlayerCharacter>(TryGetPawnOwner());
	MovementComponent = MyCharacter ? Cast<UCharacterMovementComponent>(MyCharacter->GetMovementComponent()) : nullptr;

	if (!MovementComponent)
		return;

	const FVector& Velocity = MovementComponent->Velocity;
	Speed = UKismetMathLibrary::VSizeXY(Velocity);
	bShouldMove = Speed > UPS_CharacterAnimInstance_Consts::SpeedMoveThreshold && UKismetMathLibrary::NotEqual_VectorVector(MovementComponent->GetCurrentAcceleration(), FVector::ZeroVector);
	bIsFalling = MovementComponent->IsFalling();
	Direction = UKismetAnimationLibrary::CalculateDirection(Velocity, MyCharacter->GetActorRotation());
	
}
