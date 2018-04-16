// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FPSCharacter.generated.h"

class UInputComponent;
class AFPSProjectile;
class USoundBase;
class UAnimMontage;
class USkeletalMeshComponent;
class USceneComponent;
class UCameraComponent;

UCLASS(config = Game)
class AFPSCharacter : public ACharacter
{
    GENERATED_BODY()
public:
    AFPSCharacter ();

    /** Returns Mesh1P subobject **/
    FORCEINLINE USkeletalMeshComponent* GetMesh1P () const
    {
        return mMesh;
    }
    /** Returns FirstPersonCameraComponent subobject **/
    FORCEINLINE UCameraComponent* GetFirstPersonCameraComponent () const
    {
        return mFirstPersonCameraComponent;
    }
protected:
    void BeginPlay () override;

    /** Fires a projectile. */
    void OnFire (void);

    /** Handles moving forward/backward */
    void MoveForward (const float Val);
    /** Handles stafing movement, left and right */
    void MoveRight   (const float Val);

    /**
    * Called via input to turn at a given rate.
    * @param Rate This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
    */
    void TurnAtRate   (const float Rate);
    /**
    * Called via input to turn look up/down at a given rate.
    * @param Rate This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
    */
    void LookUpAtRate (const float Rate);

    // APawn interface
    void SetupPlayerInputComponent (UInputComponent* InputComponent) override;
public:
    /** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
    UPROPERTY (VisibleAnywhere, BlueprintReadOnly, Category = "FPS|Camera")
        float                       mBaseTurnRate;
    /** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
    UPROPERTY (VisibleAnywhere, BlueprintReadOnly, Category = "FPS|Camera")
        float                       mBaseLookUpRate;
    /** Gun muzzle's offset from the characters location */
    UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = "FPS|Gameplay")
        FVector                     mGunOffset;
    /** Projectile class to spawn */
    UPROPERTY (EditDefaultsOnly, Category = "FPS|Projectile")
        TSubclassOf<AFPSProjectile> mProjectileClass;
    /** Sound to play each time we fire */
    UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = "FPS|Gameplay")
        USoundBase*                 mFireSound;
    /** AnimMontage to play each time we fire */
    UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = "FPS|Gameplay")
        UAnimMontage*               mFireAnimation;
protected:
    /** Pawn mesh: 1st person view (arms; seen only by self) */
    UPROPERTY (VisibleDefaultsOnly, Category = "FPS|Mesh")
        USkeletalMeshComponent* mMesh;
    /** Gun mesh: 1st person view (seen only by self) */
    UPROPERTY (VisibleDefaultsOnly, Category = "FPS|Mesh")
        USkeletalMeshComponent* mGun;
    /** Location on gun mesh where projectiles should spawn. */
    UPROPERTY (VisibleDefaultsOnly, Category = "FPS|Mesh")
        USceneComponent*        mMuzzleLocation;
    /** First person camera */
    UPROPERTY (VisibleAnywhere, BlueprintReadOnly, Category = "FPS|Camera", meta = (AllowPrivateAccess = "true"))
        UCameraComponent*       mFirstPersonCameraComponent;
};