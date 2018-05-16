#pragma once

#include "CoreMinimal.h"
#include "FPSCharacter.h"
#include "PlayerFPSCharacter.generated.h"

class UInputComponent;
class UCameraComponent;
class UCrosshairComponent;

/** Basic player character. */
UCLASS()
class FPS_API APlayerFPSCharacter : public AFPSCharacter
{
    GENERATED_BODY()

public:
    /** Sets default values for the APlayerFPSCharacter's properties. */
    APlayerFPSCharacter ();

    /** Sets the screen coordinates of the crosshair component. */
    void SetCrosshairScreenCoordinates (const FVector WorldCoordinates);

    /**
    * Stops any activity (shooting, jumping, moving...) and unposses the
    * character.
    */
    UFUNCTION (BlueprintCallable)
    void OnDeath () override;

    /** Called every frame. */
    void Tick (float DeltaTime) override;
protected:
    /** Called when the game starts or when spawned. */
    void BeginPlay () override;

    /** Handles moving forward/backward. */
    void MoveForward (const float Val);
    /** Handles stafing movement, left and right. */
    void MoveRight   (const float Val);

    /**
    * Called via input to turn at a given rate.
    * @param Rate This is a normalized rate, i.e. 1.0 means 100% of desired
    * turn rate.
    */
    void TurnAtRate   (const float Rate);
    /**
    * Called via input to turn look up/down at a given rate.
    * @param Rate This is a normalized rate, i.e. 1.0 means 100% of desired
    * turn rate.
    */
    void LookUpAtRate (const float Rate);

    /** APawn interface. */
    void SetupPlayerInputComponent (UInputComponent* InputComponent) override;

    /** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
    UPROPERTY (
        VisibleAnywhere,
        BlueprintReadOnly,
        Category = "FPS|Camera",
        meta = (DisplayName = "BaseTurnRate"))
    float                mBaseTurnRate;
    /** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
    UPROPERTY (
        VisibleAnywhere,
        BlueprintReadOnly,
        Category = "FPS|Camera",
        meta = (DisplayName = "BaseLookUpRate"))
    float                mBaseLookUpRate;
    /** Pointer to the first person camera */
    UPROPERTY (
        VisibleAnywhere,
        BlueprintReadOnly,
        Category = "FPS|Camera",
        meta = (AllowPrivateAccess = "true"),
        meta = (DisplayName = "FirstPersonCamera"))
    UCameraComponent*    mFirstPersonCamera;
    /** Pointer to the crosshair used by the player. */
    UPROPERTY (
        EditAnywhere,
        BlueprintReadWrite,
        Category = "FPS|UI",
        meta = (DisplayName = "Crosshair"))
    UCrosshairComponent* mCrosshair;
};
