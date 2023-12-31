#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "PS_PlayerState.generated.h"

class UPS_AbilitySystemComponent;
class UPS_PlayerAttributeSet;

UCLASS(config=Game)
class APS_PlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	APS_PlayerState();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UPS_PlayerAttributeSet* GetPlayerAttributeSet() const;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "ASC")
		UPS_AbilitySystemComponent* AbilitySystemComponent = nullptr;
	UPROPERTY(Transient)
		UPS_PlayerAttributeSet* PlayerAttributeSet = nullptr;
};

