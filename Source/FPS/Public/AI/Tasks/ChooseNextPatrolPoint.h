#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "ChooseNextPatrolPoint.generated.h"

class AAIController;
class UPatrolComponent;

/** Updates the AActor* blackboard key: PatrolPoint. */
UCLASS()
class FPS_API UChooseNextPatrolPoint : public UBTTaskNode
{
    GENERATED_BODY()

public:
    /**
    * Starts this task, should return Succeeded, Failed or InProgress
    * (use FinishLatentTask() when returning InProgress)
    * this function should be considered as const (don't modify state of
    * object) if node is not instanced!
    */
    EBTNodeResult::Type ExecuteTask (
        UBehaviorTreeComponent& OwnerComp,
        uint8*                  NodeMemory) final;
private:
    /**
    * Returns true if the AIController pointer passed is not null.
    * If not, printns a warning message in the LogTemp and returns false.
    */
    bool CheckAIController (AAIController* const AIController) const;
    /**
    * Returns true if the pawn pointer passed is not null.
    * If not, printns a warning message in the LogTemp and returns false.
    */
    bool CheckControlledPawn     (
        APawn* const         ControlledPawn,
        AAIController* const AIController) const;
    /**
    * Returns true if the patrol component pointer passed is not null.
    * If not, printns a warning message in the LogTemp and returns false.
    */
    bool CheckPatrolComponent    (
        UPatrolComponent* const PatrolComponent,
        APawn* const            ControlledPawn) const;
    /**
    * Returns true if the number passed is greater than 1.
    * If not, printns a warning message in the LogTemp and returns false.
    */
    bool CheckPatrolPointsNumber (
        const int32  NumberOfPoints,
        APawn* const ControlledPawn) const;

    /** AActor* blackboard key used to know the 'point' to investigate. */
    UPROPERTY (
        EditInstanceOnly,
        Category = "FPS",
        meta = (DisplayName = "PatrolPointKey"))
    FBlackboardKeySelector mPatrolPointKey;
};
