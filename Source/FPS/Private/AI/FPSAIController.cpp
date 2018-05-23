#include "FPSAIController.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"

#define SIGHT   0
#define HEARING 1

AFPSAIController::AFPSAIController ()
{
    PrimaryActorTick.bCanEverTick = true;

    // Create a UBlackBoardComponent.
    mBlackBoard   =
        CreateDefaultSubobject<UBlackboardComponent> (TEXT ("BlackBoard"));
    // Create a UBehaviorTreeComponent.
    mBehaviorTree =
        CreateDefaultSubobject<UBehaviorTreeComponent> (TEXT ("BehaviorTree"));
    mPerception   =
        CreateDefaultSubobject<UAIPerceptionComponent> (TEXT ("Perception"));

    // Set AFPSAIController parameters.
    bSetControlRotationFromPawnOrientation = false;
    mWeaponLocation                        = FVector{};
    mTargetStimulus                        = FAIStimulus{};
    mCurrentTarget                         = nullptr;

    // Add senses to perception.
    if (!mPerception)
    {
        UE_LOG (
            LogTemp
            , Error
            , TEXT ("Character: %s does not have an UAIPerceptionComponent.")
            , *GetName ())

            return;
    }

    // Add Sight sense.
    mSightConfig                                           =
        CreateDefaultSubobject<UAISenseConfig_Sight> (TEXT ("SigthConfig"));

    if (!mSightConfig)
    {
        UE_LOG (
            LogTemp
            , Error
            , TEXT ("Character: %s did not create an UAISenseConfig_Sight.")
            , *GetName ())

            return;
    }

    mSightConfig->SightRadius                              = 1000.f;
    mSightConfig->LoseSightRadius                          = 1500.f;
    mSightConfig->PeripheralVisionAngleDegrees             = 62.f;
    mSightConfig->DetectionByAffiliation.bDetectEnemies    = true;
    mSightConfig->DetectionByAffiliation.bDetectFriendlies = true;
    mSightConfig->DetectionByAffiliation.bDetectNeutrals   = true;
    mSightConfig->SetMaxAge (5);
    mPerception->ConfigureSense (*mSightConfig);

    // Add hearing sense.
    mHearingConfig                                           = 
        CreateDefaultSubobject<UAISenseConfig_Hearing> (TEXT ("HearingConfig"));

    if (!mHearingConfig)
    {
        UE_LOG (
            LogTemp
            , Error
            , TEXT ("Character: %s did not create an UAISenseConfig_Hearing.")
            , *GetName ())

            return;
    }

    mHearingConfig->HearingRange                             = 1500;
    mHearingConfig->DetectionByAffiliation.bDetectEnemies    = true;
    mHearingConfig->DetectionByAffiliation.bDetectFriendlies = true;
    mHearingConfig->DetectionByAffiliation.bDetectNeutrals   = true;
    mHearingConfig->SetMaxAge (3);
    mPerception->ConfigureSense (*mHearingConfig);

    // OnTargetPerceptionUpdated delegate.
    mPerception->OnTargetPerceptionUpdated.AddDynamic (
        this
        , &AFPSAIController::OnTargetPerceptionUpdated);
}

void AFPSAIController::Initialize (UBehaviorTree* BehaviorTree)
{
    check (BehaviorTree)

    mBlackBoard->InitializeBlackboard (*BehaviorTree->BlackboardAsset);
    mBehaviorTree->StartTree          (*BehaviorTree);
    BrainComponent = mBehaviorTree;

    // Set TargetKeyID, "Target" is the name of the key in the Blackboard.
    mTargetKeyID             = mBlackBoard->GetKeyID ("Target");
    mShouldInvestigateKeyID  = mBlackBoard->GetKeyID ("ShouldInvestigate");
    mPointToInvestigateKeyID = mBlackBoard->GetKeyID ("PointToInvestigate");
}

void AFPSAIController::SetWeaponLocation (const FVector WeaponLocation)
{
    mWeaponLocation = WeaponLocation;
}

void AFPSAIController::UpdateControlRotation (float DeltaTime, bool bUpdatePawn)
{
    APawn* const MyPawn{ GetPawn () };

    if (!MyPawn) return;

    FRotator       NewControlRotation{ GetControlRotation () };
    const FVector  FocalPoint        { GetFocalPoint () };

    if (FAISystem::IsValidLocation (FocalPoint))
    {
        NewControlRotation = (FocalPoint - mWeaponLocation).Rotation ();
    }
    else if (bSetControlRotationFromPawnOrientation)
    {
        NewControlRotation = MyPawn->GetActorRotation ();
    }

    // Don't pitch view unless looking at another pawn
    /*if ((NewControlRotation.Pitch != 0)
        && (Cast<APawn> (GetFocusActor ()) == nullptr))
    {
        NewControlRotation.Pitch = 0.f;
    }*/

    SetControlRotation (NewControlRotation);

    if (bUpdatePawn)
    {
        const FRotator CurrentPawnRotation{ MyPawn->GetActorRotation () };

        if (CurrentPawnRotation.Equals (NewControlRotation, 1e-3f) == false)
        {
            MyPawn->FaceRotation (NewControlRotation, DeltaTime);
        }
    }
}

void AFPSAIController::OnTargetPerceptionUpdated (
    AActor*     SensedActor,
    FAIStimulus Stimulus)
{
    APawn* const MyPawn{ GetPawn () };

    if (!MyPawn || (MyPawn->GetUniqueID () == SensedActor->GetUniqueID ()))
    {
        return;
    }

    // Check if the sensed actor is already the target.
    if (mCurrentTarget == SensedActor)
    {
        UpdateTarget (Stimulus);

        return;
    }
    // Return if the stimulus that stopped being sensed was not from the
    // current target.
    else if (!Stimulus.WasSuccessfullySensed ())
    {
        return;
    }

    if (!mCurrentTarget)
    {
        SetNewTarget (SensedActor, Stimulus);
    }
    // Prioritize targeting a player.
    else if (!mCurrentTarget->ActorHasTag ("Player")
        && SensedActor->ActorHasTag ("Player"))
    {
        SetNewTarget (SensedActor, Stimulus);
    }
    // Target the closest player.
    else if (mCurrentTarget->ActorHasTag ("Player")
        && SensedActor->ActorHasTag ("Player"))
    {
        TargetClosestPlayer (MyPawn, SensedActor, Stimulus);
    }
}

void AFPSAIController::SetNewTarget (
    AActor* const     SensedActor,
    const FAIStimulus Stimulus)
{
    // Set the current target/stimulus and focus to target.
    mCurrentTarget  = SensedActor;
    mTargetStimulus = Stimulus;

    // Update location to investigate.
    mBlackBoard->SetValue<UBlackboardKeyType_Vector> (
        mPointToInvestigateKeyID
        , Stimulus.StimulusLocation);

    if (mCurrentTarget->ActorHasTag ("Player")
        && (Stimulus.Type == SIGHT))
    {
        // Stop movement so that the AICharacter would move towards the last
        // known player location without passing through the location it was
        // moving towards.
        StopMovement ();

        // Finish investigation.
        mBlackBoard->SetValue<UBlackboardKeyType_Bool> (
            mShouldInvestigateKeyID
            , false);

        // Set the Blackboard Target.
        mBlackBoard->SetValue<UBlackboardKeyType_Object> (
            mTargetKeyID
            , SensedActor);

        SetFocus (SensedActor, EAIFocusPriority::Gameplay);
    }
    else
    {
        // Start investigation.
        mBlackBoard->SetValue<UBlackboardKeyType_Bool> (
            mShouldInvestigateKeyID
            , true);

        SetFocalPoint (Stimulus.StimulusLocation, EAIFocusPriority::Gameplay);
    }
}

void AFPSAIController::UpdateTarget (const FAIStimulus NewStimulus)
{
    // Clear target if it cannot be sensed.
    if (!NewStimulus.WasSuccessfullySensed ())
    {
        if (mTargetStimulus.Type == NewStimulus.Type) ClearTarget ();

        return;
    }

    bool IsAPlayer{ mCurrentTarget->ActorHasTag ("Player") };

    // If the current target is a player, change target stimulus in case the
    // current stimulus is 'Hearing' and the new one is 'Sight'.
    if ((IsAPlayer)
        && (mTargetStimulus.Type == HEARING)
        && (NewStimulus.Type == SIGHT))
    {
        // Update target stimulus.
        mTargetStimulus  = NewStimulus;

        // Stop movement so that the AICharacter would move towards the last
        // known player location without passing through the location it was
        // moving towards.
        StopMovement ();

        // Set the Blackboard Target.
        mBlackBoard->SetValue<UBlackboardKeyType_Object> (
            mTargetKeyID
            , mCurrentTarget);

        SetFocus (mCurrentTarget, EAIFocusPriority::Gameplay);

        // Finish investigation.
        mBlackBoard->SetValue<UBlackboardKeyType_Bool> (
            mShouldInvestigateKeyID
            , false);

        // Update location to investigate.
        mBlackBoard->SetValue<UBlackboardKeyType_Vector> (
            mPointToInvestigateKeyID
            , NewStimulus.StimulusLocation);
    }
    // If the current target is a player, and is being heared, update the
    // current 'Hearing' stimulus.
    else if ((IsAPlayer)
            && (mTargetStimulus.Type == HEARING)
            && (NewStimulus.Type == HEARING))
    {
        mBlackBoard->SetValue<UBlackboardKeyType_Vector> (
            mPointToInvestigateKeyID
            , NewStimulus.StimulusLocation);

        mTargetStimulus = NewStimulus;
    }
    // If the current target is not a player, update the location to
    // investigate.
    else if (!IsAPlayer)
    {
        // Stop movement so that the AICharacter would move towards the last
        // known stimulus location without passing through the location it
        // was moving towards.
        StopMovement ();

        mBlackBoard->SetValue<UBlackboardKeyType_Vector> (
            mPointToInvestigateKeyID
            , NewStimulus.StimulusLocation);

        mTargetStimulus = NewStimulus;
    }
}

void AFPSAIController::ClearTarget ()
{
    if (!mCurrentTarget) return;

    // If the sensed actor is a player, investigate the last known location.
    if (mCurrentTarget->ActorHasTag ("Player"))
    {
        // Stop movement so that the AICharacter would move towards the last
        // known player location without passing through the location it was
        // moving towards.
        StopMovement ();

        // Start investigating and update the location to investigate.
        mBlackBoard->SetValue<UBlackboardKeyType_Vector> (
            mPointToInvestigateKeyID
            , mTargetStimulus.StimulusLocation);
        mBlackBoard->SetValue<UBlackboardKeyType_Bool> (
            mShouldInvestigateKeyID
            , true);
    }

    ClearTargetAndFocus ();
}

void AFPSAIController::TargetClosestPlayer (
    APawn* const      MyPawn,
    AActor* const     SensedActor,
    const FAIStimulus Stimulus)
{
    if (!MyPawn) return;

    // Change target if the distance to the sensed actor is less than the
    // current target.
    if (FVector::Distance (
            MyPawn->GetActorLocation ()
            , SensedActor->GetActorLocation ())
        < FVector::Distance (
            MyPawn->GetActorLocation ()
            , mCurrentTarget->GetActorLocation ()))
    {
        SetNewTarget (SensedActor, Stimulus);
    }
}

void AFPSAIController::ClearTargetAndFocus ()
{
    mCurrentTarget  = nullptr;
    mTargetStimulus = FAIStimulus{};
    mBlackBoard->ClearValue (mTargetKeyID);
    ClearFocus (EAIFocusPriority::Gameplay);
}