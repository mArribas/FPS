// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon.h"

#include "FPSProjectile.h"
#include "components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values.
AWeapon::AWeapon ()
{
    // Set this actor to call Tick() every frame.
    // You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    // Create a gun mesh component
    mMesh =
        CreateDefaultSubobject<USkeletalMeshComponent> (TEXT ("WeaponMesh"));

    RootComponent = mMesh;

    mMuzzleLocation =
        CreateDefaultSubobject<USceneComponent> (TEXT ("MuzzleLocation"));
    mMuzzleLocation->SetupAttachment (mMesh);
    mMuzzleLocation->SetRelativeLocation (FVector (0.2f, 48.4f, -10.6f));

    // Default offset from the character location for projectiles to spawn
    mOffset = FVector (100.0f, 0.0f, 10.0f);
}

USkeletalMeshComponent* AWeapon::GetMesh (void) const
{
    return mMesh;
}

// Called when the game starts or when spawned.
void AWeapon::BeginPlay (void)
{
    Super::BeginPlay ();

}

// Called every frame.
void AWeapon::Tick (float DeltaTime)
{
    Super::Tick (DeltaTime);

}

void AWeapon::Shoot (const FRotator direction) const
{
    // Try and fire a projectile.
    if (!mProjectileClass) return;

    UWorld* const World{ GetWorld () };

    if (!World) return;

    // MuzzleOffset is in camera space, so transform it to world space before
    // offsetting from the character location to find the final muzzle position.
    const FVector  SpawnLocation{
        ((mMuzzleLocation) ?
        mMuzzleLocation->GetComponentLocation ()
            : GetActorLocation ()) + direction.RotateVector (mOffset) };

    // Set Spawn Collision Handling Override.
    FActorSpawnParameters ActorSpawnParams;
    ActorSpawnParams.SpawnCollisionHandlingOverride =
        ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

    // Spawn the projectile at the muzzle.
    World->SpawnActor<AFPSProjectile> (
        mProjectileClass
        , SpawnLocation
        , direction
        , ActorSpawnParams);

    // Try and play the sound if specified.
    if (!mFireSound) return;

    UGameplayStatics::PlaySoundAtLocation (
        this
        , mFireSound
        , GetActorLocation ());
}