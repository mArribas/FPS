#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LifeComponent.generated.h"

/** Basic life component. */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class FPS_API ULifeComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    /** Sets default values for the ULifeComponent's properties. */
    ULifeComponent ();

    /** Returns the maximum health value. */
    UFUNCTION (BlueprintCallable)
    float GetMaxHealth                  () const;
    /** Returns the current health value. */
    UFUNCTION (BlueprintCallable)
    float GetCurrentHealth              () const;
    /** Returns the percentage of current health with regard to maximum health. */
    UFUNCTION (BlueprintCallable)
    float GetCurrentHealthPercentage    () const;
    /** Returns how much health is regenerated per second. */
    UFUNCTION (BlueprintCallable)
    float GetHealthRegeneratedPerSecond () const;

    /** Returns true if the health can regenerate, false if not. */
    UFUNCTION (BlueprintCallable)
    bool CanRegenerate () const;

    /** Enables the health regeneration. */
    UFUNCTION (BlueprintCallable)
    void EnableRegeneration  ();
    /** Disables the health regeneration. */
    UFUNCTION (BlueprintCallable)
    void DisableRegeneration ();

    /**
    * If the current health is less than the maximum health, it will be
    * increased by the amount passed.
    */
    UFUNCTION (BlueprintCallable)
    void Heal   (const float AmountToHeal);
    /**
    * If the current health is greather than 0.0, it will be reduced by the
    * amount passed.
    */
    UFUNCTION (BlueprintCallable)
    void Damage (const float DamageAmount);

    /** Called every frame. */
    void TickComponent (
        float                        DeltaTime,
        ELevelTick                   TickType,
        FActorComponentTickFunction* ThisTickFunction) override;
protected:
    /** Called when the game starts. */
    void BeginPlay () override;
private:
    /** Highest value the variable mCurrentHealth can reach. */
    UPROPERTY (
        EditDefaultsOnly,
        Category = "FPS",
        meta = (DisplayName = "MaxHealth"))
    float mMaxHealth;
    /** Current health value. */
    UPROPERTY (EditAnywhere)//TODO: Remove.
    float mCurrentHealth;
    /** Amount of health to recover per second. */
    UPROPERTY (
        EditDefaultsOnly,
        Category = "FPS",
        meta = (DisplayName = "HealthRegeneratedPerSecond"))
    float mHealthRegeneratedPerSecond;
    /** Amount of time needed to pass without receive damage, to start regenerating. */
    UPROPERTY (
        EditDefaultsOnly,
        Category = "FPS",
        meta = (DisplayName = "TimeNeedToStartRegeneration"))
    float mTimeNeedToStartRegeneration;
    /** Amount of time left to start regenerating. */
    float mTimeLeftToStartRegeneration;
    /**
    * True if the health can be regenerated, false if not.
    * Depending of its value the component will tick or not.
    */
    UPROPERTY (
        EditDefaultsOnly,
        Category = "FPS",
        meta = (DisplayName = "CanRegenerate"))
    bool  mCanRegenerate;
};
