#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PS_CharacterAnimInstance.generated.h"

enum class EPS_AnimationDirection : uint8;
class UCharacterMovementComponent;
class APS_PlayerCharacter;

USTRUCT(BlueprintType)
struct FPS_AnimationDirection
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
		EPS_AnimationDirection Direction;
	UPROPERTY(EditDefaultsOnly)
		float DirectionValue = 0.f;
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnAnimationCanStop, const APS_PlayerCharacter*);
UCLASS()
class UPS_CharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	float GetDirection() const;
	EPS_AnimationDirection GetAnimationDirection();

protected:
	UPROPERTY(BlueprintReadOnly)
		float Speed = 0.f;
	UPROPERTY(BlueprintReadOnly)
		float Direction = 0.f;
	UPROPERTY(BlueprintReadOnly)
		bool bShouldMove = false;
	UPROPERTY(BlueprintReadOnly)
		bool bIsFalling = false;
	UPROPERTY(EditDefaultsOnly)
		TArray<FPS_AnimationDirection> AnimationDirections;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	UPROPERTY(Transient)
		APS_PlayerCharacter* MyCharacter = nullptr;;
	UPROPERTY(Transient)
		UCharacterMovementComponent* MovementComponent = nullptr;
};
