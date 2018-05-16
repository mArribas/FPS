#include "LifeComponent.h"

// Debug.
#include "GameFramework/Actor.h"

// Sets default values for the ULifeComponent's properties.
ULifeComponent::ULifeComponent ()
{
    // Set this component to be initialized when the game starts, and to be
    // ticked every frame.
    // You can turn these featuresoff to improve performance if you don't need
    // them.
    PrimaryComponentTick.bCanEverTick = true;

    // Set default values.
    mMaxHealth                   = 100.f;
    mHealthRegeneratedPerSecond  = 10.f;
    mTimeNeedToStartRegeneration = 3.f;
    mCanRegenerate               = false;
}


// Called when the game starts.
void ULifeComponent::BeginPlay ()
{
    UActorComponent::BeginPlay ();

    // Enable/Disable tick.
    SetComponentTickEnabled (mCanRegenerate);

    // Set initial values of mCurrentHealth and mTimeLeftToStartRegeneration.
    mCurrentHealth               = mMaxHealth;
    mTimeLeftToStartRegeneration = 0.f;

    // Look for invalid parameters.
    checkf (
        (mTimeNeedToStartRegeneration > 0.f)
        && (mMaxHealth > 0.f)
        && (mHealthRegeneratedPerSecond >= 0.f)
        , TEXT ("ERROR: Invalid parameter at the ULifeComponent from: %s")
        , *GetOwner ()->GetName ());
}


// Called every frame.
void ULifeComponent::TickComponent (
    float                        DeltaTime,
    ELevelTick                   TickType,
    FActorComponentTickFunction* ThisTickFunction)
{
    UActorComponent::TickComponent (DeltaTime, TickType, ThisTickFunction);

    if (mCurrentHealth < mMaxHealth || mTimeLeftToStartRegeneration > 0.f)
    {
        // Update mTimeLeftToStartRegeneration.
        mTimeLeftToStartRegeneration -= DeltaTime;

        if (mTimeLeftToStartRegeneration > 0.f) return;

        // Update mCurrentHealth.
        mCurrentHealth += mHealthRegeneratedPerSecond * DeltaTime;
        FMath::Clamp (mCurrentHealth, 0.0f, mMaxHealth);
    }
}

float ULifeComponent::GetMaxHealth () const
{
    return mMaxHealth;
}

float ULifeComponent::GetCurrentHealth () const
{
    return mCurrentHealth;
}

float ULifeComponent::GetCurrentHealthPercentage () const
{
    return mCurrentHealth / mMaxHealth;
}

float ULifeComponent::GetHealthRegeneratedPerSecond () const
{
    return mHealthRegeneratedPerSecond;
}

bool ULifeComponent::CanRegenerate () const
{
    return mCanRegenerate;
}

void ULifeComponent::EnableRegeneration ()
{
    mCanRegenerate = true;
    SetComponentTickEnabled (true);
}

void ULifeComponent::DisableRegeneration ()
{
    mCanRegenerate = false;
    SetComponentTickEnabled (false);
}

void ULifeComponent::Heal (const float AmountToHeal)
{
    if (mCurrentHealth < mMaxHealth)
    {
        mCurrentHealth += AmountToHeal;
        FMath::Clamp (mCurrentHealth, 0.0f, mMaxHealth);
    }
}

void ULifeComponent::Damage (const float DamageAmount)
{
    if (mCurrentHealth <= 0.f) return;

    mCurrentHealth -= DamageAmount;
    FMath::Clamp (mCurrentHealth, 0.0f, mMaxHealth);

    mTimeLeftToStartRegeneration = mTimeNeedToStartRegeneration;
}