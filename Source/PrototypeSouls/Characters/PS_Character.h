#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffectTypes.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Character.h"
#include "PS_Character.generated.h"

class UGameplayEffect;
class UPS_PlayerAttributeSet;
class UPS_GameplayAbility;
class UPS_AbilitySystemComponent;
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class UPS_InputConfigDataAsset;
struct FInputActionValue;

UCLASS(config=Game)
class APS_Character : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	FVector2D AuxMovementVector = FVector2D::ZeroVector;

	APS_Character();

	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UPS_PlayerAttributeSet* GetPlayerAttributeSet() const;
	bool IsDodging() const;
	bool IsMoving() const;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		USpringArmComponent* CameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		UCameraComponent* FollowCamera;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		UInputMappingContext* DefaultMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		UInputAction* JumpAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		UInputAction* LookAction;

	FDelegateHandle OnSpeedChangeHandle;;

	void AddCharacterAbilities();
	void OnAbilityInputPressed(FGameplayTag GameplayTag);
	void OnAbilityInputReleased(FGameplayTag GameplayTag);
	void OnCurrentSpeedChanged(const FOnAttributeChangeData& OnAttributeChangeData) const;
	void OnRemoveGameplayEffectCallback(const FActiveGameplayEffect& ActiveGameplayEffect) const;
	void InitializeAttributes() const;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "ASC")
		UPS_AbilitySystemComponent* AbilitySystemComponent = nullptr;
	UPROPERTY(EditDefaultsOnly, Category = "ASC")
		TArray<TSubclassOf<UPS_GameplayAbility>> Abilities;
	UPROPERTY(EditDefaultsOnly, Category = "ASC")
		UPS_InputConfigDataAsset* InputConfigDataAsset = nullptr;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "ASC")
		TSubclassOf<UGameplayEffect> DefaultAttributes;
	UPROPERTY(Transient)
		UPS_PlayerAttributeSet* PlayerAttributeSet = nullptr;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void StopLook();
	void StopMoving();
};

