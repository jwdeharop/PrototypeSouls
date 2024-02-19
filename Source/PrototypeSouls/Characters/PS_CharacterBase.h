#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "PS_CharacterBase.generated.h"

class UWidgetComponent;
class UPS_GameplayAbility;
class UPS_BaseAttributeSet;
class UPS_AbilitySystemComponent;
class UGameplayEffect;

UCLASS(config=Game)
class APS_CharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	APS_CharacterBase(const FObjectInitializer& ObjectInitializer);
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UPS_BaseAttributeSet* GetPlayerAttributeSet() const;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "ASC")
		UPS_AbilitySystemComponent* AbilitySystemComponent = nullptr;
	UPROPERTY(EditDefaultsOnly, Category = "ASC")
		TArray<TSubclassOf<UPS_GameplayAbility>> Abilities;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "ASC")
		TSubclassOf<UGameplayEffect> DefaultAttributes;
	UPROPERTY(EditDefaultsOnly)
		UWidgetComponent* LockWidgetComponent = nullptr;
	UPROPERTY(Transient)
		UPS_BaseAttributeSet* PlayerAttributeSet = nullptr;

	virtual void BeginPlay() override;
	
	void AddCharacterAbilities();
	void InitializeAttributes() const;
};

