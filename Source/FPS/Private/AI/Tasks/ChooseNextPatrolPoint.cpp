#include "ChooseNextPatrolPoint.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Public/Components/PatrolComponent.h"

EBTNodeResult::Type UChooseNextPatrolPoint::ExecuteTask (
    UBehaviorTreeComponent& OwnerComp,
    uint8*                  NodeMemory)
{
    // Get the patrol points.
    AAIController* const AIController   { OwnerComp.GetAIOwner () };
    if (!CheckAIController (AIController))
    {
        return EBTNodeResult::Failed;
    }
    APawn*               ControlledPawn { AIController->GetPawn () };
    if (!CheckControlledPawn (ControlledPawn, AIController))
    {
        return EBTNodeResult::Failed;
    }
    UPatrolComponent*    PatrolComponent{
        ControlledPawn->FindComponentByClass<UPatrolComponent> () };
    if (!CheckPatrolComponent (PatrolComponent, ControlledPawn))
    {
        return EBTNodeResult::Failed;
    }
    TArray<AActor*>      PatrolPoints   { PatrolComponent->GetPatrolPoints () };
    if (!CheckPatrolPointsNumber (PatrolPoints.Num (), ControlledPawn))
    {
        return EBTNodeResult::Failed;
    }

    // Set the patrol point.
    OwnerComp.GetBlackboardComponent ()->SetValueAsObject (
        mPatrolPointKey.SelectedKeyName
        , PatrolPoints[PatrolComponent->GetNextPatrolPoint ()]);

    // Update patrol component.
    PatrolComponent->Update ();

    return EBTNodeResult::Succeeded;
}

bool UChooseNextPatrolPoint::CheckAIController (
    AAIController* const AIController) const
{
    if (AIController) return true;

    UE_LOG (
        LogTemp
        , Warning
        , TEXT ("No AIController found."))

    return false;
}

bool UChooseNextPatrolPoint::CheckControlledPawn (
    APawn* const         ControlledPawn,
    AAIController* const AIController) const
{
    check (AIController)

    if (ControlledPawn) return true;

    UE_LOG (
        LogTemp
        , Warning
        , TEXT ("%s: No controlled pawn found.")
        , *AIController->GetName ())

    return false;
}

bool UChooseNextPatrolPoint::CheckPatrolComponent (
    UPatrolComponent* const PatrolComponent,
    APawn* const            ControlledPawn) const
{
    check (ControlledPawn)

    if (PatrolComponent) return true;

    UE_LOG (
        LogTemp
        , Warning
        , TEXT ("%s: No patrol component found.")
        , *ControlledPawn->GetName ())

    return false;
}

bool UChooseNextPatrolPoint::CheckPatrolPointsNumber (
    const int32  NumberOfPoints,
    APawn* const ControlledPawn) const
{
    check (ControlledPawn)

    if (NumberOfPoints > 1) return true;

    UE_LOG (
        LogTemp
        , Warning
        , TEXT ("%s: Number of patrol points lesser than 2.")
        , *ControlledPawn->GetName ())

    return false;
}