#include "PatrolComponent.h"

// Sets default values for the UPatrolComponent's properties.
UPatrolComponent::UPatrolComponent ()
{
    // Set this component to be initialized when the game starts, and to be
    // ticked every frame.
    // You can turn these features off to improve performance if you don't need
    // them.
    PrimaryComponentTick.bCanEverTick = false;
    mNextPatrolPoint                  = 0;
}

TArray<AActor*> UPatrolComponent::GetPatrolPoints () const
{
    return mPatrolPoints;
}

int32 UPatrolComponent::GetNextPatrolPoint () const
{
    return mNextPatrolPoint;
}

void UPatrolComponent::Update ()
{
    mNextPatrolPoint = (mNextPatrolPoint + 1) % mPatrolPoints.Num ();
}
