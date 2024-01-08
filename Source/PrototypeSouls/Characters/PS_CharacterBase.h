#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "PS_CharacterBase.generated.h"

class UPS_GameplayAbility;
class UPS_PlayerAttributeSet;
class UPS_AbilitySystemComponent;
class UGameplayEffect;

UCLASS(config=Game)
class APS_CharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UPS_PlayerAttributeSet* GetPlayerAttributeSet() const;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "ASC")
		UPS_AbilitySystemComponent* AbilitySystemComponent = nullptr;
	UPROPERTY(EditDefaultsOnly, Category = "ASC")
		TArray<TSubclassOf<UPS_GameplayAbility>> Abilities;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "ASC")
		TSubclassOf<UGameplayEffect> DefaultAttributes;
	UPROPERTY(Transient)
		UPS_PlayerAttributeSet* PlayerAttributeSet = nullptr;
	
	void AddCharacterAbilities();
	void InitializeAttributes() const;
};

