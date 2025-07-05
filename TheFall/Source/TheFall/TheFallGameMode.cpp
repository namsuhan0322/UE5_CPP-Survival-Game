// Copyright Epic Games, Inc. All Rights Reserved.

#include "TheFallGameMode.h"
#include "BaseClass/TFPlayerCharacter.h"
#include "UObject/ConstructorHelpers.h"

ATheFallGameMode::ATheFallGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
