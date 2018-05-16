#include "Shoot.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Public/Characters/FPSCharacter.h"

EBTNodeResult::Type UShoot::ExecuteTask (
    UBehaviorTreeComponent& OwnerComp,
    uint8*                  NodeMemory)
{
    UBlackboardComponent* const BlackboardComponent{
        OwnerComp.GetBlackboardComponent () };
    AFPSCharacter* const        Self{
        Cast<AFPSCharacter> (
            BlackboardComponent->GetValueAsObject (
                mSelfActorKey.SelectedKeyName)) };

    if (!Self) return EBTNodeResult::Failed;

    // If the AI was moving to the player, it will shoot while moving until
    // reaching a point under the acceptable radius, unless StopMovement() is
    // called.
    Self->Shoot ();

    return EBTNodeResult::Succeeded;
}