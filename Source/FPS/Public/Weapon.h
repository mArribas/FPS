// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

class USkeletalMeshComponent;
class AFPSProjectile;
class USoundBase;
class USceneComponent;

UCLASS()
class FPS_API AWeapon : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for the AWeapon properties.
    AWeapon ();

    USkeletalMeshComponent* GetMesh (void) const;

    // Called every frame.
    void Tick (float DeltaTime) override;

    void Shoot (const FRotator direction) const;
protected:
    // Called when the game starts or when spawned.
    void BeginPlay (void) override;

    /** Gun mesh: 1st person view (seen only by self) */
    UPROPERTY (VisibleDefaultsOnly, Category = "FPS|Mesh")
        USkeletalMeshComponent*     mMesh;
    /** Location on gun mesh where projectiles should spawn. */
    UPROPERTY (VisibleDefaultsOnly, Category = "FPS|Mesh")
        USceneComponent*            mMuzzleLocation;
    /** Gun muzzle's offset from the characters location */
    UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = "FPS|Gameplay")
        FVector                     mOffset;
    /** Projectile class to spawn */
    UPROPERTY (EditDefaultsOnly, Category = "FPS|Projectile")
        TSubclassOf<AFPSProjectile> mProjectileClass;
    /** Sound to play each time we fire */
    UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = "FPS|Gameplay")
        USoundBase*                 mFireSound;
};
