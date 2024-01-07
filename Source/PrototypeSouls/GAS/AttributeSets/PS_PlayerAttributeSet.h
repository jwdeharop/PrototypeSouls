#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "PS_PlayerAttributeSet.generated.h"

// Uses macros from AttributeSet.h
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class UPS_PlayerAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, Category = "ASC | Attributes", ReplicatedUsing = OnRep_CurrentHealth)
		FGameplayAttributeData CurrentHealth;
	ATTRIBUTE_ACCESSORS(UPS_PlayerAttributeSet, CurrentHealth);

	UPROPERTY(BlueprintReadOnly, Category = "ASC | Attributes", ReplicatedUsing = OnRep_MaxWalkSpeed)
		FGameplayAttributeData MaxWalkSpeed;
	ATTRIBUTE_ACCESSORS(UPS_PlayerAttributeSet, MaxWalkSpeed)

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	UFUNCTION()
		void OnRep_CurrentHealth(const FGameplayAttributeData& OldHealth) const;
	UFUNCTION()
		void OnRep_MaxWalkSpeed(const FGameplayAttributeData& OlMaxWalkSpeed) const;
};