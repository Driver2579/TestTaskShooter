// Copyright Epic Games, Inc. All Rights Reserved.

#include "TestTaskShooterGameMode.h"
#include "TestTaskShooterCharacter.h"
#include "UObject/ConstructorHelpers.h"

ATestTaskShooterGameMode::ATestTaskShooterGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));

	if (PlayerPawnBPClass.Class != NULL) {
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
