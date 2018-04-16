// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FPSCharacter.generated.h"

class UInputComponent;
class UAnimMontage;
class USkeletalMeshComponent;
class UCameraComponent;
class AWeapon;

UCLASS(config = Game)
class AFPSCharacter : public ACharacter
{
    GENERATED_BODY()
public:
    AFPSCharacter ();

    /** Returns Mesh1P subobject **/
    FORCEINLINE USkeletalMeshComponent* GetMesh () const
    {
        return mMesh;
    }
    /** Returns FirstPersonCameraComponent subobject **/
    FORCEINLINE UCameraComponent* GetFirstPersonCameraComponent () const
    {
        return mFirstPersonCameraComponent;
    }

    UFUNCTION (BlueprintCallable)
        void PickWeapon (TSubclassOf<AWeapon> weapon);
protected:
    void BeginPlay (void) override;

    /** Shoots a projectile. */
    void Shoot (void);

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
    /** AnimMontage to play each time we fire */
    UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = "FPS|Gameplay")
        UAnimMontage*               mFireAnimation;
protected:
    /** Pawn mesh: 1st person view (arms; seen only by self) */
    UPROPERTY (VisibleDefaultsOnly, Category = "FPS|Mesh")
        USkeletalMeshComponent* mMesh;
    /** First person camera */
    UPROPERTY (VisibleAnywhere, BlueprintReadOnly, Category = "FPS|Camera", meta = (AllowPrivateAccess = "true"))
        UCameraComponent*       mFirstPersonCameraComponent;
    /** First person camera */
    UPROPERTY (VisibleDefaultsOnly, Category = "FPS|Weapon", meta = (AllowPrivateAccess = "true"))
        AWeapon*                mWeapon;
};