#include "Weapon.h"

#include "GameFramework/Actor.h"
#include "components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "sound/SoundBase.h"
#include "TimerManager.h"
#include "Perception/AISense_Hearing.h"

// Debug.
#include "DrawDebugHelpers.h"

// Sets default values.
AWeapon::AWeapon ()
{
    // Set this actor to call Tick() every frame.
    // You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    // Create a gun mesh component.
    mMesh =
        CreateDefaultSubobject<USkeletalMeshComponent> (TEXT ("WeaponMesh"));

    RootComponent = mMesh;

    // Create a scene component to locate the muzzle.
    mMuzzleLocation =
        CreateDefaultSubobject<USceneComponent> (TEXT ("MuzzleLocation"));
    mMuzzleLocation->SetupAttachment (mMesh);
    mMuzzleLocation->SetWorldRotation (FRotator{ 0.f, 90.f, 0.f });

    // Set weapon properties.

    mDamage                = 20.f;
    mOffset                = FVector{ 0.0f, 52.f, 27.f };
    mShootRate             = 0.3f;
    mProjectileMaxDistance = 10000.f;
    mCanShoot              = true;
    mHitResult             = FHitResult{};
    AddIgnoredActorToHitParams (this);
}

USkeletalMeshComponent* AWeapon::GetMesh (void) const
{
    return mMesh;
}

FHitResult AWeapon::GetHitResult () const
{
    return mHitResult;
}

void AWeapon::AddIgnoredActorToHitParams (const AActor* Actor)
{
    mHitparams.AddIgnoredActor (Actor);
}

// Called when the game starts or when spawned.
void AWeapon::BeginPlay ()
{
    AActor::BeginPlay ();

    if (!mMuzzleLocation) return;

    // Set Muzzle location.
    if (mMesh->GetSocketByName ("Muzzle"))
    {
        mMuzzleLocation->SetWorldLocation (mMesh->GetSocketLocation ("Muzzle"));
    }
    else
    {
        mMuzzleLocation->SetRelativeLocation (mOffset);
        UE_LOG (LogTemp, Error, TEXT ("Weapon needs muzzle socket."))
    }
}

// Called every frame.
void AWeapon::Tick (float DeltaTime)
{
    AActor::Tick (DeltaTime);

    // Update HitResult.
    FVector EndPoint{
        mMuzzleLocation->GetComponentLocation ()
        + mMuzzleLocation->GetComponentRotation ().Vector ()
        * mProjectileMaxDistance };
    bool    DidHit  {
        GetWorld ()->LineTraceSingleByChannel (
            mHitResult
            , mMuzzleLocation->GetComponentLocation ()
            , EndPoint
            , ECC_Visibility
            , mHitparams) };

    // Update mHitActor.
    if (!DidHit)
    {
        mHitActor = nullptr;

        DrawDebugLine (
            GetWorld ()
            , mMuzzleLocation->GetComponentLocation ()
            , EndPoint
            , FColor::Blue
            , false
            , -1
            , 0
            , 1.f
        );
    }
    else
    {
        mHitActor = mHitResult.GetActor ();

        DrawDebugLine (
            GetWorld ()
            , mMuzzleLocation->GetComponentLocation ()
            , mHitResult.Location
            , FColor::Red
            , false
            , -1
            , 0
            , 1.f
        );
    }
}

void AWeapon::Shoot ()
{
    UWorld* World{ GetWorld () };

    if (!World || !mCanShoot) return;

    mCanShoot = false;

    // Set shoot timer.
    World->GetTimerManager ().SetTimer (
        mShootCooldownEndHandle
        , this
        , &AWeapon::OnShootCooldownEnd
        , mShootRate
        , true);

    // Play the sound if specified.
    if (mShootSound)
    {
        UGameplayStatics::PlaySoundAtLocation (
            this
            , mShootSound
            , GetActorLocation ());
    }

    // Make a noise at the muzzle location.
    UAISense_Hearing::ReportNoiseEvent (
        World
        , mMuzzleLocation->GetComponentLocation ()
        , 2.f
        , Instigator);

    if (!mHitActor) return;

    // Hit reactions.
    // Make a noise at the hit location.
    UAISense_Hearing::ReportNoiseEvent (
        World
        , mHitResult.Location
        , 1.f
        , mHitActor);

    // Play the sound if specified.
    if (mHitSound)
    {
        UGameplayStatics::PlaySoundAtLocation (
            this
            , mHitSound
            , GetActorLocation ());
    }

    // Damage the actor.
    FPointDamageEvent PointDamage;
    PointDamage.HitInfo         = mHitResult;
    PointDamage.ShotDirection   =
        mMuzzleLocation->GetComponentRotation ().Vector ();
    PointDamage.Damage          = mDamage;

    mHitActor->TakeDamage (
        PointDamage.Damage
        , PointDamage
        , Instigator->Controller
        , this);
}

void AWeapon::OnShootCooldownEnd ()
{
    mCanShoot = true;

    // Clear timer to avoid continuous calls.
    if (GetWorld ())
    {
        GetWorld ()->GetTimerManager ().ClearTimer (mShootCooldownEndHandle);
    }
}