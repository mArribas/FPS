// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Shoot.generated.h"

/** Shoots at the target. */
UCLASS()
class FPS_API UShoot : public UBTTaskNode
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
    /** AActor* blackboard key used to know the AI actor. */
    UPROPERTY (
        EditInstanceOnly,
        Category = "FPS",
        meta = (DisplayName = "SelfActor"))
        FBlackboardKeySelector mSelfActorKey;
};
