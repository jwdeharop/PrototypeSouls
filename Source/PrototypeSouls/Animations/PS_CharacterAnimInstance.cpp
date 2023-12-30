#include "Animations/PS_CharacterAnimInstance.h" 
#include "Characters/PS_Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

namespace UPS_CharacterAnimInstance_Consts
{
	static constexpr float SpeedMoveThreshold = 3.f;
}

void UPS_CharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	MyCharacter = Cast<APS_Character>(TryGetPawnOwner());
	MovementComponent = MyCharacter ? Cast<UCharacterMovementComponent>(MyCharacter->GetMovementComponent()) : nullptr;

	if (!MovementComponent)
		return;

	Speed = UKismetMathLibrary::VSizeXY(MovementComponent->Velocity);
	bShouldMove = Speed > UPS_CharacterAnimInstance_Consts::SpeedMoveThreshold && UKismetMathLibrary::NotEqual_VectorVector(MovementComponent->GetCurrentAcceleration(), FVector::ZeroVector);
	bIsFalling = MovementComponent->IsFalling();
	
}
