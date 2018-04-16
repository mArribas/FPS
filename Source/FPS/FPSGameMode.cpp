// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "FPSGameMode.h"
#include "FPSHUD.h"
#include "FPSCharacter.h"
#include "UObject/ConstructorHelpers.h"

AFPSGameMode::AFPSGameMode ()
    : Super ()
{
    // Set default pawn class to our Blueprinted character.
    static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder{
        TEXT ("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter") };
    DefaultPawnClass = PlayerPawnClassFinder.Class;

    // Use our custom HUD class.
    HUDClass = AFPSHUD::StaticClass ();
}
