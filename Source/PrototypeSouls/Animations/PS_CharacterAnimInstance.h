#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PS_CharacterAnimInstance.generated.h"

class UCharacterMovementComponent;
class APS_Character;

UCLASS()
class UPS_CharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly)
		float Speed = 0.f;
	UPROPERTY(BlueprintReadOnly)
		float Direction = 0.f;
	UPROPERTY(BlueprintReadOnly)
		bool bShouldMove = false;
	UPROPERTY(BlueprintReadOnly)
		bool bIsFalling = false;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	UPROPERTY(Transient)
		APS_Character* MyCharacter = nullptr;;
	UPROPERTY(Transient)
		UCharacterMovementComponent* MovementComponent = nullptr;
};
