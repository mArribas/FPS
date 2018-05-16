#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "StopInvestigating.generated.h"

/** Updates the boolen blackboard key: ShouldInvestigate. */
UCLASS()
class FPS_API UStopInvestigating : public UBTTaskNode
{
    GENERATED_BODY()

public:
    /**
    * starts this task, should return Succeeded, Failed or InProgress
    * (use FinishLatentTask() when returning InProgress)
    * this function should be considered as const (don't modify state of
    * object) if node is not instanced!
    */
    EBTNodeResult::Type ExecuteTask (
        UBehaviorTreeComponent& OwnerComp,
        uint8*                  NodeMemory) final;
private:
    /**
    * Boolean blackboard key used to know if it is necessary to investigate a
    * location.
    */
    UPROPERTY (
        EditInstanceOnly,
        Category = "FPS",
        meta = (DisplayName = "ShouldInvestigateKey"))
    FBlackboardKeySelector mShouldInvestigateKey;
};
