// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "FPSCharacter.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "Public/Weapon.h"

DEFINE_LOG_CATEGORY_STATIC (LogFPChar, Warning, All);

AFPSCharacter::AFPSCharacter ()
{
    // Set size for collision capsule.
    GetCapsuleComponent ()->InitCapsuleSize (30.f, 96.0f);

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
        CreateDefaultSubobject<USkeletalMeshComponent> (TEXT ("CharacterMesh"));
    mMesh->SetOnlyOwnerSee (true);
    mMesh->SetupAttachment (mFirstPersonCameraComponent);
    mMesh->bCastDynamicShadow = false;
    mMesh->CastShadow         = false;
    mMesh->RelativeRotation   = FRotator{ 1.9f, -19.19f, 5.2f };
    mMesh->RelativeLocation   = FVector { -0.5f, -4.4f, -155.7f };

    mWeapon = nullptr;
}

void AFPSCharacter::BeginPlay (void)
{
    // Call the base class  
    Super::BeginPlay ();
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
        "Shoot"
        , IE_Pressed
        , this
        , &AFPSCharacter::Shoot);

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

void AFPSCharacter::PickWeapon (TSubclassOf<AWeapon> weapon)
{
    if (!weapon || mWeapon) return;

    mWeapon                                 =
        GetWorld ()->SpawnActor<AWeapon> (
            weapon
            , FVector{ 0, 0, 0 }
            , FRotator::ZeroRotator);

    mWeapon->GetMesh ()->bCastDynamicShadow = false;
    mWeapon->GetMesh ()->CastShadow         = false;
    // Attach weapon to Skeleton.
    mWeapon->AttachToComponent (
        mMesh
        , FAttachmentTransformRules{ EAttachmentRule::SnapToTarget, true }
        , TEXT ("GripPoint"));
}

void AFPSCharacter::Shoot (void)
{
    // Try shooting the weapon.
    if (!mWeapon) return;

    mWeapon->Shoot (GetControlRotation ());

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
