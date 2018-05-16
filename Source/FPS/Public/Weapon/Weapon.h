#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

class USkeletalMeshComponent;
class USoundBase;
class USceneComponent;

/** Basic fire weapon. */
UCLASS()
class FPS_API AWeapon : public AActor
{
    GENERATED_BODY()

public:
    /** Sets default values for the AWeapon's properties. */
    AWeapon ();

    /** Returns the weapon skeletal mesh component. */
    USkeletalMeshComponent* GetMesh      () const;
    /** Returns the weapon hit result. */
    FHitResult              GetHitResult () const;

    /** Adds an actor to ignore to the hit params. */
    void AddIgnoredActorToHitParams (const AActor* Actor);

    /** Called every frame to update the hit result and hit actor. */
    void Tick (float DeltaTime) override;

    /**
    * If the weapon can shoot, apply the weapon sound and reports a noise at
    * the muzzle location. If mHitActor is not null, it will also apply the
    * hit sound, report a noise at the hit location and damage the actor.
    */
    void Shoot ();
protected:
    /** Called when the game starts or when spawned. */
    void BeginPlay () override;

    /** Sets mCanShoot to true (called the mShootCooldownEndHandle timer ends). */
    virtual void OnShootCooldownEnd ();

    /** Weapon mesh. */
    UPROPERTY (
        VisibleDefaultsOnly,
        Category = "FPS|Mesh",
        meta = (DisplayName = "Mesh"))
    USkeletalMeshComponent*     mMesh;
    /** World location of the weapon muzzle. */
    UPROPERTY (
        VisibleDefaultsOnly,
        Category = "FPS|Gameplay",
        meta = (DisplayName = "MuzzleLocation"))
    USceneComponent*            mMuzzleLocation;
    /** Damage done by the weapon. */
    UPROPERTY (
        EditAnywhere,
        BlueprintReadWrite,
        Category = "FPS|Gameplay",
        meta = (DisplayName = "Damage"))
    float                       mDamage;
    /** Gun muzzle's offset from the characters location. */
    UPROPERTY (
        EditAnywhere,
        BlueprintReadWrite,
        Category = "FPS|Gameplay",
        meta = (DisplayName = "Offset"))
    FVector                     mOffset;
    /** Sound to play each time we shoot. */
    UPROPERTY (
        EditAnywhere,
        BlueprintReadWrite,
        Category = "FPS|Gameplay",
        meta = (DisplayName = "ShootSound"))
    USoundBase*                 mShootSound;
    /** Fire rate of the weapon. */
    UPROPERTY (
        EditAnywhere,
        BlueprintReadWrite,
        Category = "FPS|Gameplay",
        meta = (DisplayName = "ShootRate"))
    float                       mShootRate;
    /** Sound to play each time the shoot hits something. */
    UPROPERTY (
        EditAnywhere,
        BlueprintReadWrite,
        Category = "FPS|Gameplay",
        meta = (DisplayName = "HitSound"))
    USoundBase*                 mHitSound;
    /** Maximum distance a projectile can reach. */
    UPROPERTY (
        EditDefaultsOnly,
        Category = "FPS|Projectile",
        meta = (DisplayName = "ProjectileMaxDistance"))
    float                       mProjectileMaxDistance;
    /** Hit result of the raycast used to know where the weapon is aiming. */
    FHitResult                  mHitResult;
    /** Hit params from mHitResult. */
    FCollisionQueryParams       mHitparams;
    /** Pointer to mHitResult actor. */
    AActor*                     mHitActor;
    /** Used to know if the weapon can shoot. */
    bool                        mCanShoot;
    /** Timer used to call OnShootCooldownEnd () when the weapon cooldown ends. */
    FTimerHandle                mShootCooldownEndHandle;
};
