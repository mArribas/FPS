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

    // Default offset from the character location for projectiles to spawn
    //mOffset = FVector (100.0f, 0.0f, 10.0f);
    mOffset = FVector{ 17.f, -7.5f, 17.5f };
}

USkeletalMeshComponent* AWeapon::GetMesh (void) const
{
    return mMesh;
}

// Called when the game starts or when spawned.
void AWeapon::BeginPlay (void)
{
    Super::BeginPlay ();

    if (!mMuzzleLocation) return;

    /*if (mMesh->GetSocketByName ("Muzzle"))
        mMuzzleLocation->SetWorldLocation (mMesh->GetSocketLocation ("Muzzle"));
    else*/
        mMuzzleLocation->SetRelativeLocation (FVector{ (0.f, 53.f, 11.25f) });
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

    // Set Spawn Collision Handling Override.
    FActorSpawnParameters ActorSpawnParams;
    ActorSpawnParams.SpawnCollisionHandlingOverride =
        ESpawnActorCollisionHandlingMethod::DontSpawnIfColliding;
        //ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

    // Spawn the projectile at the muzzle.
    World->SpawnActor<AFPSProjectile> (
        mProjectileClass
        , mMuzzleLocation->GetComponentLocation ()
        , direction
        , ActorSpawnParams);

    // Try and play the sound if specified.
    if (!mFireSound) return;

    UGameplayStatics::PlaySoundAtLocation (
        this
        , mFireSound
        , GetActorLocation ());
}