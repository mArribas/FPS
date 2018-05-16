#include "FPSAIController.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"

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
        // Clear target if it cannot be sensed.
        if (!Stimulus.WasSuccessfullySensed ())
        {
            ClearTarget (SensedActor, Stimulus);
        }
        else
        {
            // If the sensed actor is a player, change target stimulus in case
            // the current stimulus is 'Hearing' and the new one is 'Sight'.
            if ((mCurrentTarget->ActorHasTag ("Player"))
                && (mTargetStimulus.Type == 1)
                && (Stimulus.Type == 0))
            {
                // Stop movement so that the AICharacter would move towards the last
                // known player location without passing through the location it was
                // moving towards.
                StopMovement ();

                // Update location to investigate.
                mBlackBoard->SetValue<UBlackboardKeyType_Vector> (
                    mPointToInvestigateKeyID
                    , Stimulus.StimulusLocation);

                // Change target stimulus.
                mTargetStimulus = Stimulus;
            }
            // If the sensed actor is a player, update the location to
            // investigate.
            else if (!mCurrentTarget->ActorHasTag ("Player"))
            {
                mBlackBoard->SetValue<UBlackboardKeyType_Vector> (
                    mPointToInvestigateKeyID
                    , Stimulus.StimulusLocation);
            }
        }

        return;
    }
    else if (!Stimulus.WasSuccessfullySensed ())
    {
        return;
    }

    if (!mCurrentTarget) SetTarget (SensedActor, Stimulus);

    // Return if the target is a player and the sensed actor is not.
    if (mCurrentTarget->ActorHasTag ("Player")
        && !SensedActor->ActorHasTag ("Player"))
    {
        return;
    }

    // Prioritize targeting a player.
    if (!mCurrentTarget->ActorHasTag ("Player")
        && SensedActor->ActorHasTag ("Player"))
    {
        SetTarget (SensedActor, Stimulus);

        return;
    }

    // Target the closest player.
    TargetClosestPlayer (MyPawn, SensedActor, Stimulus);
}

void AFPSAIController::SetTarget (
    AActor* const     SensedActor,
    const FAIStimulus Stimulus)
{
    // Set the current target and focus to it.
    mCurrentTarget  = SensedActor;
    SetFocus (SensedActor, EAIFocusPriority::Gameplay);
    mTargetStimulus = Stimulus;

    // If the target is a player finish the investigation.
    if (mCurrentTarget->ActorHasTag ("Player"))
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
    }
    else
    {
        // Start investigation.
        mBlackBoard->SetValue<UBlackboardKeyType_Bool> (
            mShouldInvestigateKeyID
            , true);
    }

    // Update location to investigate.
    mBlackBoard->SetValue<UBlackboardKeyType_Vector> (
        mPointToInvestigateKeyID
        , Stimulus.StimulusLocation);
}

void AFPSAIController::ClearTarget (
    AActor* const     SensedActor,
    const FAIStimulus Stimulus)
{
    if (!mCurrentTarget) return;

    // Check that the stimulus passed is of the same type as the target
    // stimulus.
    if (!(mTargetStimulus.Type == Stimulus.Type)) return;

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
            , Stimulus.StimulusLocation);
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
        SetTarget (SensedActor, Stimulus);
    }
}

void AFPSAIController::ClearTargetAndFocus ()
{
    mCurrentTarget = nullptr;
    mBlackBoard->ClearValue (mTargetKeyID);
    ClearFocus (EAIFocusPriority::Gameplay);
}