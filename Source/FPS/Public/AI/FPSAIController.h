#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "FPSAIController.generated.h"

class UBlackboardComponent;
class UBehaviorTreeComponent;
class UBehaviorTree;
class UAIPerceptionComponent;
class UAISenseConfig_Sight;
class UAISenseConfig_Hearing;

/** Basic AI controller. */
UCLASS()
class FPS_API AFPSAIController : public AAIController
{
    GENERATED_BODY()

public:
    AFPSAIController ();

    /**
    * Initializes the blackboard component and starts the behaviour tree
    * component.
    */
    void Initialize (UBehaviorTree* BehaviorTree);

    /** Sets the weapon location. */
    void SetWeaponLocation (const FVector Location);

    /** Updates the controller rotation. */
    void UpdateControlRotation     (
        float DeltaTime,
        bool  bUpdatePawn = true) override;
    /**
    * Called each time an actor is sensed or stopped being sensed.
    * Updates the current target and the stimulus associated with it.
    */
    UFUNCTION ()
    void OnTargetPerceptionUpdated (AActor* SensedActor, FAIStimulus Stimulus);
private:
    /**
    * Sets the current target and the stimulus associated with it, to the
    * actor pointer and stimulus passed.
    */
    void SetTarget           (
        AActor* const     SensedActor,
        const FAIStimulus Stimulus);
    /**
    * Clears the current target if the actor pointer and stimulus passed are
    * the current target and the stimulus associated with it.
    */
    void ClearTarget         (
        AActor* const     SensedActor,
        const FAIStimulus Stimulus);
    /** Sets the closest 'player' to the controlled pawn as the current target. */
    void TargetClosestPlayer (
        APawn* const      MyPawn,
        AActor* const     SensedActor,
        const FAIStimulus Stimulus);

    /** Clears mCurrentTarget as well as the Blackboard Target and the focus. */
    void ClearTargetAndFocus ();

    /** Blackboard component.*/
    UPROPERTY (transient)
    UBlackboardComponent*   mBlackBoard;
    /** Behavior tree component.*/
    UPROPERTY (transient)
    UBehaviorTreeComponent* mBehaviorTree;
    /** AI perception component. */
    UPROPERTY (
        EditDefaultsOnly,
        meta = (DisplayName = "Perception"))
    UAIPerceptionComponent* mPerception;
    /** Sight sense configuration. */
    UAISenseConfig_Sight*   mSightConfig;
    /** Hearing sense configuration. */
    UAISenseConfig_Hearing* mHearingConfig;
    /**
    * Used as a reference location to update the controller location when is
    * focusing at an actor.
    */
    FVector                 mWeaponLocation;
    /** Stimulus associated with mCurrentTarget. */
    FAIStimulus             mTargetStimulus;
    /** Current target of the AI. */
    AActor*                 mCurrentTarget;
    /** ID of the blackboard Target key.*/
    uint8                   mTargetKeyID;
    /** ID of the blackboard ShouldInvestigate key.*/
    uint8                   mShouldInvestigateKeyID;
    /** ID of the blackboard PointToInvestigate key.*/
    uint8                   mPointToInvestigateKeyID;
};
