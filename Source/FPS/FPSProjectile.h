// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FPSProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;

UCLASS(config = Game)
class AFPSProjectile : public AActor
{
    GENERATED_BODY()

public:
    AFPSProjectile ();

    /** called when projectile hits something */
    UFUNCTION ()
        void OnHit (
            UPrimitiveComponent* HitComp,
            AActor*              OtherActor,
            UPrimitiveComponent* OtherComp,
            FVector              NormalImpulse,
            const FHitResult&    Hit);

    /** Returns CollisionComp subobject **/
    FORCEINLINE USphereComponent* GetCollisionComp () const
    {
        return mCollisionComp;
    }
    /** Returns ProjectileMovement subobject **/
    FORCEINLINE UProjectileMovementComponent* GetProjectileMovement () const
    {
        return mProjectileMovement;
    }
private:
    /** Sphere collision component */
    UPROPERTY (VisibleDefaultsOnly, Category = "FPS|Projectile")
        USphereComponent*             mCollisionComp;
    /** Projectile movement component */
    UPROPERTY (VisibleAnywhere, BlueprintReadOnly, Category = "FPS|Projectile", meta = (AllowPrivateAccess = "true"))
        UProjectileMovementComponent* mProjectileMovement;
};
