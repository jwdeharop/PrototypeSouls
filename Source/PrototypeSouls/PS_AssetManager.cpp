#include "PS_AssetManager.h"
#include "AbilitySystemGlobals.h"

UPS_AssetManager& UPS_AssetManager::Get()
{
	if (UPS_AssetManager* Singleton = Cast<UPS_AssetManager>(GEngine->AssetManager))
		return *Singleton;

	UE_LOG(LogTemp, Fatal, TEXT("Invalid AssetManager in DefaultEngine.ini, must be UPS_AssetManager"));
	return *NewObject<UPS_AssetManager>();
}

void UPS_AssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	UAbilitySystemGlobals::Get().InitGlobalData();
}
