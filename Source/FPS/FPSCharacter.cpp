// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "FPSCharacter.h"
#include "FPSProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC (LogFPChar, Warning, All);

AFPSCharacter::AFPSCharacter ()
{
    // Set size for collision capsule.
    GetCapsuleComponent ()->InitCapsuleSize (55.f, 96.0f);

    // Set our turn rates for input.
    mBaseTurnRate   = 45.f;
    mBaseLookUpRate = 45.f;

    // Create a CameraComponent.
    mFirstPersonCameraComponent                          =
        CreateDefaultSubobject<UCameraComponent> (TEXT ("FirstPersonCamera"));
    mFirstPersonCameraComponent->SetupAttachment (GetCapsuleComponent ());
    // Position the camera.
    mFirstPersonCameraComponent->RelativeLocation        =
        FVector{ -39.56f, 1.75f, 64.f };
    mFirstPersonCameraComponent->bUsePawnControlRotation = true;

    // Create a mesh component that will be used when being viewed from a '1st
    // person' view (when controlling this pawn).
    mMesh                     =
        CreateDefaultSubobject<USkeletalMeshComponent> (TEXT ("CharacterMesh1P"));
    mMesh->SetOnlyOwnerSee (true);
    mMesh->SetupAttachment (mFirstPersonCameraComponent);
    mMesh->bCastDynamicShadow = false;
    mMesh->CastShadow         = false;
    mMesh->RelativeRotation   = FRotator{ 1.9f, -19.19f, 5.2f };
    mMesh->RelativeLocation   = FVector { -0.5f, -4.4f, -155.7f };

    // Create a gun mesh component
    mGun                     =
        CreateDefaultSubobject<USkeletalMeshComponent> (TEXT ("FP_Gun"));
    // Only the owning player will see this mesh.
    mGun->SetOnlyOwnerSee (true);
    mGun->bCastDynamicShadow = false;
    mGun->CastShadow         = false;
    mGun->SetupAttachment(mMesh, TEXT("GripPoint"));
    //mGun->SetupAttachment (RootComponent);

    mMuzzleLocation =
        CreateDefaultSubobject<USceneComponent> (TEXT ("MuzzleLocation"));
    mMuzzleLocation->SetupAttachment (mGun);
    mMuzzleLocation->SetRelativeLocation (FVector (0.2f, 48.4f, -10.6f));

    // Default offset from the character location for projectiles to spawn
    mGunOffset = FVector (100.0f, 0.0f, 10.0f);
}

void AFPSCharacter::BeginPlay ()
{
    // Call the base class  
    Super::BeginPlay ();

    // Attach gun mesh component to Skeleton, doing it here because the skeleton
    // is not yet created in the constructor.
    mGun->AttachToComponent (
        mMesh
        , FAttachmentTransformRules{ EAttachmentRule::SnapToTarget, true }
        , TEXT ("GripPoint"));
}

void AFPSCharacter::SetupPlayerInputComponent (
    UInputComponent* PlayerInputComponent)
{
    // Set up gameplay key bindings.
    check (PlayerInputComponent);

    // Bind jump events.
    PlayerInputComponent->BindAction (
        "Jump"
        , IE_Pressed
        , this
        , &ACharacter::Jump);
    PlayerInputComponent->BindAction (
        "Jump"
        , IE_Released
        , this
        , &ACharacter::StopJumping);

    // Bind fire event.
    PlayerInputComponent->BindAction (
        "Fire"
        , IE_Pressed
        , this
        , &AFPSCharacter::OnFire);

    // Bind movement events.
    PlayerInputComponent->BindAxis (
        "MoveForward"
        , this
        , &AFPSCharacter::MoveForward);
    PlayerInputComponent->BindAxis (
        "MoveRight"
        , this
        , &AFPSCharacter::MoveRight);

    // We have 2 versions of the rotation bindings to handle different kinds
    // of devices differently:
    //      - "turn" handles devices that provide an absolute delta, such as a 
    //        mouse.
    //      - "turnrate" is for devices that we choose to treat as a rate of
    //        change, such as an analog joystick
    PlayerInputComponent->BindAxis (
        "Turn"
        , this
        , &APawn::AddControllerYawInput);
    PlayerInputComponent->BindAxis (
        "TurnRate"
        , this
        , &AFPSCharacter::TurnAtRate);
    PlayerInputComponent->BindAxis (
        "LookUp"
        , this
        , &APawn::AddControllerPitchInput);
    PlayerInputComponent->BindAxis (
        "LookUpRate"
        , this
        , &AFPSCharacter::LookUpAtRate);
}

void AFPSCharacter::OnFire ()
{
    // Try and fire a projectile.
    if (!mProjectileClass) return;

    UWorld* const World{ GetWorld () };

    if (!World) return;

    const FRotator SpawnRotation{ GetControlRotation () };
    // MuzzleOffset is in camera space, so transform it to world space before
    // offsetting from the character location to find the final muzzle position.
    const FVector  SpawnLocation{ 
        ((mMuzzleLocation) ?
            mMuzzleLocation->GetComponentLocation ()
            : GetActorLocation ()) + SpawnRotation.RotateVector (mGunOffset) };

    // Set Spawn Collision Handling Override.
    FActorSpawnParameters ActorSpawnParams;
    ActorSpawnParams.SpawnCollisionHandlingOverride =
        ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

    // Spawn the projectile at the muzzle.
    World->SpawnActor<AFPSProjectile> (
        mProjectileClass
        , SpawnLocation
        , SpawnRotation
        , ActorSpawnParams);

    // Try and play the sound if specified.
    if (mFireSound)
    {
        UGameplayStatics::PlaySoundAtLocation (
            this
            , mFireSound
            , GetActorLocation ());
    }

    // Get the animation object for the arms mesh.
    UAnimInstance* AnimInstance{ mMesh->GetAnimInstance () };

    // Try and play a firing animation if specified.
    if (!mFireAnimation || !AnimInstance) return;

    AnimInstance->Montage_Play (mFireAnimation, 1.f);
}

void AFPSCharacter::MoveForward (const float Value)
{
    // Add movement in that direction.
    if (Value != 0.0f) AddMovementInput (GetActorForwardVector (), Value);
}

void AFPSCharacter::MoveRight (const float Value)
{
    // Add movement in that direction.
    if (Value != 0.0f) AddMovementInput (GetActorRightVector (), Value);
}

void AFPSCharacter::TurnAtRate (const float Rate)
{
    // Calculate delta for this frame from the rate information.
    AddControllerYawInput (
        Rate * mBaseTurnRate * GetWorld ()->GetDeltaSeconds ());
}

void AFPSCharacter::LookUpAtRate (const float Rate)
{
    // Calculate delta for this frame from the rate information.
    AddControllerPitchInput (
        Rate * mBaseLookUpRate * GetWorld ()->GetDeltaSeconds ());
}
