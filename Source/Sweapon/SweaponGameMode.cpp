// Copyright Epic Games, Inc. All Rights Reserved.

#include "SweaponGameMode.h"
#include "SweaponCharacter.h"
#include "UObject/ConstructorHelpers.h"

ASweaponGameMode::ASweaponGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
