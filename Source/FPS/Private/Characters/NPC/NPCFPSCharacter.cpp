#include "NPCFPSCharacter.h"

#include "Public/AI/FPSAIController.h"
#include "Public/Weapon/Weapon.h"

ANPCFPSCharacter::ANPCFPSCharacter () : AFPSCharacter ()
{
    // Add NPC tag.
    Tags.Add ("NPC");
}

void ANPCFPSCharacter::BeginPlay ()
{
    if (!mNPCBehavior)
    {
        UE_LOG (
            LogTemp
            , Warning
            , TEXT ("NPC: %s does not have behavior tree.")
            , *GetName ())
        Destroy ();

        return;
    }

    AFPSAIController* AIController{ Cast<AFPSAIController> (Controller) };

    if (!AIController)
    {
        UE_LOG (
            LogTemp
            , Warning
            , TEXT ("NPC: %s controller is not a AFPSAIController.")
            , *GetName ())
        Destroy ();

        return;
    }

    AIController->Initialize (mNPCBehavior);

    // Call the base class.
    AFPSCharacter::BeginPlay ();
}

void ANPCFPSCharacter::Tick (float DeltaTime)
{
    if (!mWeapon) return;

    AFPSCharacter::Tick (DeltaTime);

    AFPSAIController* AIController{ Cast<AFPSAIController> (Controller) };

    if (!AIController) return;

    AIController->SetWeaponLocation (mWeapon->GetActorLocation ());
}