#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "CheckLife.generated.h"

/** Checks if the AI is still alive an updates the IsDeath blackboard key.*/
UCLASS()
class FPS_API UCheckLife : public UBTService
{
    GENERATED_BODY()
public:
    UCheckLife ();

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
    /** Boolean blackboard key used to know the AI is alive. */
    UPROPERTY (
        EditInstanceOnly,
        Category = "FPS",
        meta = (DisplayName = "IsDeath"))
        FBlackboardKeySelector mIsDeathKey;
};
