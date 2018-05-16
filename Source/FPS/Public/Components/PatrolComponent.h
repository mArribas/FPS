#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PatrolComponent.generated.h"

/** Basic patrol component. */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class FPS_API UPatrolComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    /** Sets default values for the UPatrolComponent's properties. */
    UPatrolComponent ();

    /** Returns an array with the actors used to locate the positions to patrol. */
    UFUNCTION (BlueprintCallable)
    TArray<AActor*> GetPatrolPoints    () const;
    /** Returns the index of the next 'point' to patrol. */
    UFUNCTION (BlueprintCallable)
    int32           GetNextPatrolPoint () const;

    /** Updates the index of the next patrol point. */
    void Update ();
private:
    /** Array with the actors which location should be patrolled. */
    UPROPERTY (
        EditInstanceOnly,
        Category = "FPS",
        meta = (DisplayName = "PatrolPoints"))
    TArray<AActor*> mPatrolPoints;
    /** Index of the next patrol point. */
    UPROPERTY (
        EditInstanceOnly,
        Category = "FPS",
        meta = (DisplayName = "NextPatrolPoint"))
    int32           mNextPatrolPoint;
};
