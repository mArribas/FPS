#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "CheckDistanceToPlayerTarget.generated.h"

/**
* Checks if the AI target is a player, then if the player is at shooting
* distance, and updates the blackboard key IsAtShootingDistance.
*/
UCLASS()
class FPS_API UCheckDistanceToPlayerTarget : public UBTService
{
    GENERATED_BODY()

public:
    UCheckDistanceToPlayerTarget ();

    /**
    * Update next tick interval (this function should be considered as const
    * (don't modify state of object) if node is not instanced!).
    */
    virtual void TickNode (
        UBehaviorTreeComponent& OwnerComp,
        uint8*                  NodeMemory,
        float                   DeltaSeconds) override;
private:
    /** AActor* blackboard key used to know the AI actor. */
    UPROPERTY (
        EditInstanceOnly,
        Category = "FPS",
        meta = (DisplayName = "SelfActor"))
        FBlackboardKeySelector mSelfActorKey;
    /** AActor* blackboard key used to know the AI target. */
    UPROPERTY (
        EditInstanceOnly,
        Category = "FPS",
        meta = (DisplayName = "Target"))
        FBlackboardKeySelector mTargetKey;
    /** Boolean blackboard key used to know if it can shoot. */
    UPROPERTY (
        EditInstanceOnly,
        Category = "FPS",
        meta = (DisplayName = "IsAtShootingDistance"))
        FBlackboardKeySelector mIsAtShootingDistanceKey;
    /** Distance at which the AI can shoot. */
    UPROPERTY (
        EditInstanceOnly,
        Category = "FPS",
        meta = (DisplayName = "ShootingRange"))
    float mShootingRange;
};
