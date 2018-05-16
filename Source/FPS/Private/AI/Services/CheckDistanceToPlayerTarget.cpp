#include "CheckDistanceToPlayerTarget.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Public/Characters/FPSCharacter.h"

UCheckDistanceToPlayerTarget::UCheckDistanceToPlayerTarget ()
{
    bCreateNodeInstance = true;
    mShootingRange      = 700.f;
}

void UCheckDistanceToPlayerTarget::TickNode (
    UBehaviorTreeComponent& OwnerComp,
    uint8*                  NodeMemory,
    float                   DeltaSeconds)
{
    UBlackboardComponent* const BlackboardComponent{
        OwnerComp.GetBlackboardComponent () };
    AFPSCharacter* const        Self               {
        Cast<AFPSCharacter> (
            BlackboardComponent->GetValueAsObject (
                mSelfActorKey.SelectedKeyName)) };
    AActor* const               Target             {
        Cast<AActor> (
            BlackboardComponent->GetValueAsObject (
                mTargetKey.SelectedKeyName)) };
    bool                        LastShootingState  {
        BlackboardComponent->GetValueAsBool (
            mIsAtShootingDistanceKey.SelectedKeyName) };
    bool                        NewShootingState   { false };

    if ((!Self) || (!Target) || (!Target->ActorHasTag ("Player")))
    {
        BlackboardComponent->SetValueAsBool (
            mIsAtShootingDistanceKey.SelectedKeyName
            , NewShootingState);

        if (LastShootingState) Self->StopShooting ();

        return;
    }

    // Check if the player is at shooting distance.
    if (Self->GetDistanceTo (Target) <= mShootingRange) NewShootingState = true;

    // Stop shooting if the state passed from shooting to not shooting.
    if (LastShootingState && !NewShootingState) Self->StopShooting ();

    BlackboardComponent->SetValueAsBool (
        mIsAtShootingDistanceKey.SelectedKeyName
        , NewShootingState);
}