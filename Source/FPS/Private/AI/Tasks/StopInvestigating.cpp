#include "StopInvestigating.h"

#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UStopInvestigating::ExecuteTask (
    UBehaviorTreeComponent& OwnerComp,
    uint8*                  NodeMemory)
{
    OwnerComp.GetBlackboardComponent ()->SetValueAsBool (
        mShouldInvestigateKey.SelectedKeyName
        , false);

    return EBTNodeResult::Succeeded;
}
