#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "GameplayTagContainer.h"
#include "PS_PlayerState.h"
#include "Characters/PS_CharacterBase.h"
#include "Interfaces/PS_DamageableInterface.h"
#include "PS_PlayerCharacter.generated.h"

class UPS_PlayerCameraComponent;
struct FPS_ComboWeaponInfo;
class UPS_WeaponComboConfig;
class APS_Weapon;
class UGameplayEffect;
class UPS_BaseAttributeSet;
class UPS_GameplayAbility;
class UPS_AbilitySystemComponent;
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class UPS_InputConfigDataAsset;
struct FInputActionValue;

UCLASS(config=Game)
class APS_PlayerCharacter : public APS_CharacterBase, public IPS_DamageableInterface
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = Weapons)
		TSubclassOf<APS_Weapon> InitialWeapon = nullptr;
	UPROPERTY(Replicated)
		FVector2D AuxMovementVector = FVector2D::ZeroVector;

	bool bCanChangeCombo = false;

	APS_PlayerCharacter(const FObjectInitializer& ObjectInitializer);

	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE UPS_PlayerCameraComponent* GetFollowCamera() const { return FollowCamera; }

	APS_Weapon* GetCurrentWeapon() const;

	float GetMaxSpeed() const;
	bool IsDodging() const;
	bool IsMoving() const;

	FPS_ComboWeaponInfo GetCurrentWeaponComboInfo() const;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		USpringArmComponent* CameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		UPS_PlayerCameraComponent* FollowCamera;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		UInputMappingContext* DefaultMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		UInputAction* JumpAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		UInputAction* LookAction;

	UFUNCTION(Server, Reliable)
		void Server_SetAuxMovementVector(const FVector2D& MovementVector);
	UFUNCTION(Server, Reliable)
		void Server_SetUsesRotationYaw(bool bUsesRotationYaw);

	void OnAbilityInputPressed(FGameplayTag GameplayTag);
	void OnAbilityInputReleased(FGameplayTag GameplayTag);
	void OnCurrentSpeedChanged(const FOnAttributeChangeData& OnAttributeChangeData) const;
	void OnControllerGetsPlayerState(APS_PlayerState* APSPlayerState);
	void OnAttackFinished(FGameplayTag GameplayTag, int NewCount);
	void OnLightAttackDamage(FGameplayTag GameplayTag, int NewCount);
	void RegisterAbilitySystemComponentBindings();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "ASC")
		UPS_InputConfigDataAsset* InputConfigDataAsset = nullptr;
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
		UPS_WeaponComboConfig* WeaponComboConfig = nullptr;
	UPROPERTY(Transient, Replicated)
		TWeakObjectPtr<APS_Weapon> CurrentWeapon = nullptr;
	UPROPERTY(EditDefaultsOnly)
		UPS_DamageableComponent* DamageableComponent = nullptr;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void OnRep_PlayerState() override;
	virtual UPS_DamageableComponent* GetDamageableComponent() override;

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void StopMoving();
};

