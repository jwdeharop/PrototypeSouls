#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "PS_BaseAttributeSet.generated.h"

// Uses macros from AttributeSet.h
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class UPS_BaseAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, Category = "ASC | Attributes", ReplicatedUsing = OnRep_CurrentHealth)
		FGameplayAttributeData CurrentHealth;
	ATTRIBUTE_ACCESSORS(UPS_BaseAttributeSet, CurrentHealth);

	UPROPERTY(BlueprintReadOnly, Category = "ASC | Attributes", ReplicatedUsing = OnRep_CurrentArmor)
		FGameplayAttributeData Armor;
	ATTRIBUTE_ACCESSORS(UPS_BaseAttributeSet, Armor);

	UPROPERTY(BlueprintReadOnly, Category = "ASC | Attributes", ReplicatedUsing = OnRep_Damage)
		FGameplayAttributeData Damage;
	ATTRIBUTE_ACCESSORS(UPS_BaseAttributeSet, Damage);

	UPROPERTY(BlueprintReadOnly, Category = "ASC | Attributes", ReplicatedUsing = OnRep_MaxWalkSpeed)
		FGameplayAttributeData MaxWalkSpeed;
	ATTRIBUTE_ACCESSORS(UPS_BaseAttributeSet, MaxWalkSpeed)

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	UFUNCTION()
		void OnRep_CurrentHealth(const FGameplayAttributeData& OldHealth) const;
	UFUNCTION()
		void OnRep_MaxWalkSpeed(const FGameplayAttributeData& OlMaxWalkSpeed) const;
	UFUNCTION()
		void OnRep_CurrentArmor(const FGameplayAttributeData& OldArmor) const;
	UFUNCTION()
		void OnRep_Damage(const FGameplayAttributeData& OldDamage);
};