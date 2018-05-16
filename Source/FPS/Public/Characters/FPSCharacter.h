#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FPSCharacter.generated.h"

class USkeletalMeshComponent;
class AWeapon;
class ULifeComponent;

UCLASS(abstract)
class AFPSCharacter : public ACharacter
{
    GENERATED_BODY()
public:
    AFPSCharacter ();

    /** Sets the weapon equipped. */
    UFUNCTION (BlueprintCallable)
    void SetWeapon                     (
        TSubclassOf<AWeapon> Weapon, FName GripPoint);

    /** Returns the pointer to the weapon equipped. */
    UFUNCTION (BlueprintCallable)
    AWeapon* GetWeapon  () const;

    /** Returns the value of mIsShooting. */
    UFUNCTION (BlueprintCallable)
    bool IsShooting () const;
    /** Returns the value of mIsJumping. */
    UFUNCTION (BlueprintCallable)
    bool IsJumping  () const;

    /** Calls the Jump function from ACharacter, and sets mIsJumping to true. */
    void Jump        () override;
    /**
    * Calls the StopJumping function from ACharacter, and sets mIsJumping to
    * true.
    */
    void StopJumping () override;

    /**
    * If the current health of the life component is greather than 0.0 returns
    * true. If not, returns false. In case the character does not have a life
    * component, it will always return true.
    */
    UFUNCTION (BlueprintCallable)
    bool IsDeath () const;

    /**
    * Stops any activity (shooting, jumping, moving...) and unposses the
    * character.
    */
    UFUNCTION (BlueprintCallable)
    virtual void OnDeath ();

    /** Calls the Shoot function from AWeapon, and sets mIsShooting to true. */
    void Shoot        ();
    /** Sets mIsShooting to false. */
    void StopShooting ();

    /** UE4 damage system function. */
    float TakeDamage (
        float               Damage,
        FDamageEvent const& DamageEvent,
        AController*        EventInstigator,
        AActor*             DamageCauser) override;

    /** Called every frame. */
    void Tick (float DeltaTime) override;
protected:
    /** Pointer to the AWeapon equipped.*/
    UPROPERTY (
        VisibleDefaultsOnly,
        Category = "FPS|Weapon",
        meta = (AllowPrivateAccess = "true"),
        meta = (DisplayName = "Weapon"))
    AWeapon*          mWeapon;
    /** Pointer to the ULifeComponent. */
    UPROPERTY (
        EditDefaultsOnly,
        BlueprintReadOnly,
        Category = "FPS|Life",
        meta = (AllowPrivateAccess = "true"),
        meta = (DisplayName = "Life"))
    ULifeComponent*   mLife;
    /** Used to know if the character is shooting. */
    bool              mIsShooting;
    /** Used to know if the character is jumping. */
    bool              mIsJumping;
    /** Used to know if the character is death. */
    bool              mIsDeath;
};