#include "Libraries/PS_NetLibrary.h"

bool UPS_NetLibrary::IsServer(const UObject* WorldContextObject)
{
	const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	return World && World->GetNetMode() < ENetMode::NM_Client;
}

bool UPS_NetLibrary::IsDedicatedServer(const UObject* WorldContextObject)
{
	const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	return World && World->GetNetMode() == ENetMode::NM_DedicatedServer;
}

bool UPS_NetLibrary::IsClient(const UObject* WorldContextObject)
{
	const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	return World && World->GetNetMode() != ENetMode::NM_DedicatedServer;
}

bool UPS_NetLibrary::IsSimulatedClient(const APawn* Pawn)
{
	return Pawn && Pawn->GetLocalRole() == ENetRole::ROLE_SimulatedProxy;
}
