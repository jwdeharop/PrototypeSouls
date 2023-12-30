// Copyright Epic Games, Inc. All Rights Reserved.

#include "PrototypeSoulsGameMode.h"
#include "PrototypeSoulsCharacter.h"
#include "UObject/ConstructorHelpers.h"

APrototypeSoulsGameMode::APrototypeSoulsGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
