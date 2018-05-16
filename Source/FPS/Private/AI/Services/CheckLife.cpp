#include "CheckLife.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Public/Characters/FPSCharacter.h"

UCheckLife::UCheckLife ()
{
    bCreateNodeInstance = true;
}

void UCheckLife::TickNode (
    UBehaviorTreeComponent& OwnerComp,
    uint8*                  NodeMemory,
    float                   DeltaSeconds)
{
    UBlackboardComponent* const BlackboardComponent{
        OwnerComp.GetBlackboardComponent () };
    AFPSCharacter* const        Self{
        Cast<AFPSCharacter> (
            BlackboardComponent->GetValueAsObject (
                mSelfActorKey.SelectedKeyName)) };
    if (!Self)          return;

    BlackboardComponent->SetValueAsBool (
        mIsDeathKey.SelectedKeyName
        , Self->IsDeath ());
}