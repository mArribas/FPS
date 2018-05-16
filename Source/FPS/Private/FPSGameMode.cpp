// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "Public/FPSGameMode.h"
#include "Public/HUD/FPSHUD.h"
#include "Public/Characters/FPSCharacter.h"
#include "UObject/ConstructorHelpers.h"

AFPSGameMode::AFPSGameMode ()
    : Super ()
{
    // Set default pawn class to our Blueprinted character.
    static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder{
        TEXT ("/Game/Blueprints/Characters/Player/BP_PlayerFPSCharacter") };
    DefaultPawnClass = PlayerPawnClassFinder.Class;

    // Use our custom HUD class.
    HUDClass = AFPSHUD::StaticClass ();
}
