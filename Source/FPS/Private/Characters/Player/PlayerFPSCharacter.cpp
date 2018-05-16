#include "PlayerFPSCharacter.h"

#include "GameFramework/Actor.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "Kismet/GameplayStatics.h"
#include "Public/HUD/FPSHUD.h"
#include "Public/Weapon/Weapon.h"
#include "Public/Components/CrosshairComponent.h"

APlayerFPSCharacter::APlayerFPSCharacter () : AFPSCharacter ()
{
    // Add player tag.
    Tags.Add ("Player");

    // Set our turn rates for input.
    mBaseTurnRate   = 45.f;
    mBaseLookUpRate = 45.f;

    // Create a camera component.
    mFirstPersonCamera                          =
        CreateDefaultSubobject<UCameraComponent> (TEXT ("FirstPersonCamera"));
    mFirstPersonCamera->SetupAttachment (GetCapsuleComponent ());
    // Camera position.
    mFirstPersonCamera->RelativeLocation        =
        FVector{ 20.f, 12.f, 70.f };
    /*mFirstPersonCameraComponent->RelativeLocation        =
    FVector{ 34.f, 0.f, 70.f };*/
    mFirstPersonCamera->bUsePawnControlRotation = true;

    // Create crosshair component.
    mCrosshair =
        CreateDefaultSubobject<UCrosshairComponent> (TEXT ("Crosshair"));
}

void APlayerFPSCharacter::BeginPlay ()
{
    // Call the base class.
    AFPSCharacter::BeginPlay ();

    APlayerController* PlayerController{
        GetWorld ()->GetFirstPlayerController () };

    if (!mCrosshair || !PlayerController) return;

    PlayerController->PlayerCameraManager->ViewPitchMax = 30.f;
    PlayerController->PlayerCameraManager->ViewPitchMin = -35.f;

    // Set HUD crosshair.
    AFPSHUD* HUD{
        Cast<AFPSHUD> (PlayerController->GetHUD ()) };

    if (!HUD)
    {
        UE_LOG (LogTemp, Error, TEXT ("Failed setting HUD crosshair."))

        return;
    }

    HUD->SetCrosshair (mCrosshair);
}

void APlayerFPSCharacter::Tick (float DeltaTime)
{
    if (!mWeapon) return;

    // Update crosshair screen coordinates.
    SetCrosshairScreenCoordinates (mWeapon->GetHitResult ().Location);

    Super::Tick (DeltaTime);
}

void APlayerFPSCharacter::SetupPlayerInputComponent (
    UInputComponent* PlayerInputComponent)
{
    // Set up gameplay key bindings.
    check (PlayerInputComponent);

    // Bind jump events.
    PlayerInputComponent->BindAction (
        "Jump"
        , IE_Pressed
        , this
        , &AFPSCharacter::Jump);
    PlayerInputComponent->BindAction (
        "Jump"
        , IE_Released
        , this
        , &AFPSCharacter::StopJumping);

    // Bind shoot event.
    PlayerInputComponent->BindAction (
        "Shoot"
        , IE_Pressed
        , this
        , &AFPSCharacter::Shoot);
    PlayerInputComponent->BindAction (
        "Shoot"
        , IE_Released
        , this
        , &AFPSCharacter::StopShooting);

    // Bind movement events.
    PlayerInputComponent->BindAxis (
        "MoveForward"
        , this
        , &APlayerFPSCharacter::MoveForward);
    PlayerInputComponent->BindAxis (
        "MoveRight"
        , this
        , &APlayerFPSCharacter::MoveRight);

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
        , &APlayerFPSCharacter::TurnAtRate);
    PlayerInputComponent->BindAxis (
        "LookUp"
        , this
        , &APawn::AddControllerPitchInput);
    PlayerInputComponent->BindAxis (
        "LookUpRate"
        , this
        , &APlayerFPSCharacter::LookUpAtRate);
}

void APlayerFPSCharacter::SetCrosshairScreenCoordinates (
    const FVector WorldCoordinates)
{
    if (!GetWorld ()) return;

    FVector2D ScreenCoordinates{};
    // Get screen coordinates.
    bool      Success{
        UGameplayStatics::ProjectWorldToScreen (
            GetWorld ()->GetFirstPlayerController ()
            , WorldCoordinates
            , ScreenCoordinates) };

    if (Success) mCrosshair->SetScreenCoordinates (ScreenCoordinates);
}

void APlayerFPSCharacter::OnDeath ()
{
    AFPSCharacter::OnDeath ();

    Tags.Remove ("Player");
}

void APlayerFPSCharacter::MoveForward (const float Value)
{
    // Add movement in that direction.
    if (Value != 0.0f) AddMovementInput (GetActorForwardVector (), Value);
}

void APlayerFPSCharacter::MoveRight (const float Value)
{
    // Add movement in that direction.
    if (Value != 0.0f) AddMovementInput (GetActorRightVector (), Value);
}

void APlayerFPSCharacter::TurnAtRate (const float Rate)
{
    // Calculate delta for this frame from the rate information.
    AddControllerYawInput (
        Rate * mBaseTurnRate * GetWorld ()->GetDeltaSeconds ());
}

void APlayerFPSCharacter::LookUpAtRate (const float Rate)
{
    // Calculate delta for this frame from the rate information.
    AddControllerPitchInput (
        Rate * mBaseLookUpRate * GetWorld ()->GetDeltaSeconds ());
}
