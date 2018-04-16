// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "FPSProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"

AFPSProjectile::AFPSProjectile ()
{
    // Use a sphere as a simple collision representation.
    mCollisionComp                       =
        CreateDefaultSubobject<USphereComponent> (TEXT ("SphereComp"));
    mCollisionComp->InitSphereRadius (5.0f);
    mCollisionComp->BodyInstance.SetCollisionProfileName ("Projectile");
    // Set up a notification for when this component hits something blocking.
    mCollisionComp->OnComponentHit.AddDynamic (this, &AFPSProjectile::OnHit);
    // Players can't walk on it.
    mCollisionComp->SetWalkableSlopeOverride (
        FWalkableSlopeOverride (WalkableSlope_Unwalkable, 0.f));
    mCollisionComp->CanCharacterStepUpOn = ECB_No;

    // Set as root component.
    RootComponent = mCollisionComp;

    // Use a ProjectileMovementComponent to govern this projectile's movement.
    mProjectileMovement                           =
        CreateDefaultSubobject<UProjectileMovementComponent> (TEXT ("ProjectileComp"));
    mProjectileMovement->UpdatedComponent         = mCollisionComp;
    mProjectileMovement->InitialSpeed             = 3000.f;
    mProjectileMovement->MaxSpeed                 = 3000.f;
    mProjectileMovement->bRotationFollowsVelocity = true;
    mProjectileMovement->bShouldBounce            = true;

    // Die after 3 seconds by default.
    InitialLifeSpan = 3.0f;
}

void AFPSProjectile::OnHit (
    UPrimitiveComponent* HitComp,
    AActor*              OtherActor,
    UPrimitiveComponent* OtherComp,
    FVector              NormalImpulse,
    const FHitResult&    Hit)
{
    // Only add impulse and destroy projectile if we hit a physics.
    if (!OtherActor
        || (OtherActor == this)
        || !OtherComp
        || !OtherComp->IsSimulatingPhysics ()) return;

    OtherComp->AddImpulseAtLocation (
        GetVelocity () * 100.0f
        , GetActorLocation ());

    Destroy ();
}