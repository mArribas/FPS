#include "FPSCharacter.h"

#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Public/Weapon/Weapon.h"
#include "Public/Components/LifeComponent.h"

DEFINE_LOG_CATEGORY_STATIC (LogFPChar, Warning, All);

AFPSCharacter::AFPSCharacter ()
{
    // Set size for collision capsule.
    GetCapsuleComponent ()->InitCapsuleSize (45.f, 96.0f);

    mWeapon     = nullptr;
    mIsShooting = false;
    mIsJumping  = false;
    mIsDeath    = false;

    USkeletalMeshComponent* Mesh{ GetMesh () };

    checkf (
        Mesh
        , TEXT ("ERROR: %s does not have a mesh.")
        , *GetName ());

    // Set mesh properties.
    Mesh->SetOnlyOwnerSee (false);
    Mesh->bCastDynamicShadow = false;
    Mesh->CastShadow         = true;
    Mesh->RelativeRotation   = FRotator{ 0.f, -90.f, 0.f };
    Mesh->RelativeLocation   = FVector { 0.f, 0.f, -98.f };

    // Create a life component.
    mLife =
        CreateDefaultSubobject<ULifeComponent> (TEXT ("Life"));
}

void AFPSCharacter::Tick (float DeltaTime)
{
    Super::Tick (DeltaTime);

    if      (!mLife)                           mIsDeath = false;
    else if (mLife->GetCurrentHealth () > 0.f) mIsDeath = false;
    else if (!mIsDeath)                        OnDeath ();

    if (mIsShooting) Shoot ();
}

void AFPSCharacter::SetWeapon (TSubclassOf<AWeapon> Weapon, FName GripPoint)
{
    if (!Weapon || mWeapon) return;

    // Spawn the weapon passed in the world.
    mWeapon                                 =
        GetWorld ()->SpawnActor<AWeapon> (
            Weapon
            , FVector{ 0, 0, 0 }
            , FRotator::ZeroRotator);
    // Set weapon properties.
    mWeapon->AddIgnoredActorToHitParams (this);
    mWeapon->Instigator = this;
    // Attach weapon to Skeleton.
    if (GetMesh ()->GetSocketByName (GripPoint))
    {
        mWeapon->AttachToComponent (
            GetMesh ()
            , FAttachmentTransformRules{ EAttachmentRule::SnapToTarget, true }
            , GripPoint);
    }
    else
    {
        UE_LOG (
            LogTemp
            , Error
            , TEXT ("Character: %s does not have a socket named: %s.")
            , *GetName ()
            , *GripPoint.ToString ())
    }
}

AWeapon* AFPSCharacter::GetWeapon () const
{
    return mWeapon;
}

bool AFPSCharacter::IsShooting () const
{
    return mIsShooting;
}

bool AFPSCharacter::IsJumping () const
{
    return mIsJumping;
}

void AFPSCharacter::Jump ()
{
    ACharacter::Jump ();

    mIsJumping = true;
}

void AFPSCharacter::StopJumping ()
{
    ACharacter::StopJumping ();

    mIsJumping = false;
}

bool AFPSCharacter::IsDeath () const
{
    return mIsDeath;
}

void AFPSCharacter::OnDeath ()
{
    mIsDeath = true;

    if (mIsShooting) StopShooting ();
    if (mIsJumping)  StopJumping  ();

    Controller->StopMovement ();
    Controller->UnPossess    ();

    SetActorTickEnabled (false);

    // To avoid blocking other characters.
    GetCapsuleComponent ()->SetCollisionProfileName (TEXT ("Spectator"));
}

void AFPSCharacter::Shoot ()
{
    // Try shooting the weapon.
    if (!mWeapon) return;

    mIsShooting = true;

    mWeapon->Shoot ();
}

void AFPSCharacter::StopShooting ()
{
    mIsShooting = false;
}

float AFPSCharacter::TakeDamage (
    float               Damage,
    FDamageEvent const& DamageEvent,
    AController*        EventInstigator,
    AActor*             DamageCauser)
{
    if (!mLife) return 0.f;

    mLife->Damage (Damage);

    return Damage;
}
