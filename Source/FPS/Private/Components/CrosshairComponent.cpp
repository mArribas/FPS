#include "CrosshairComponent.h"

// Sets default values for the CrosshairComponent's properties.
UCrosshairComponent::UCrosshairComponent ()
{
    // Set this component to be initialized when the game starts, and to be
    // ticked every frame.
    // You can turn these features off to improve performance if you don't need
    // them.
    PrimaryComponentTick.bCanEverTick = false;

    mCrosshairTexWidth  = 64;
    mCrosshairTexHeight = 64;
    mScreenCoordinates  = FVector2D{ 0.f, 0.f };
}

void UCrosshairComponent::SetCrosshairTexture (
    UTexture2D* Texture,
    uint8       Width,
    uint8       Height)
{
    if (!Texture) return;

    mCrosshairTex       = Texture;
    mCrosshairTexWidth  = Width;
    mCrosshairTexHeight = Height;
}

void UCrosshairComponent::SetScreenCoordinates (const FVector2D ScreenCoordinates)
{
    mScreenCoordinates = ScreenCoordinates;
}

UTexture2D* UCrosshairComponent::GetTexture () const
{
    return mCrosshairTex;
}

uint8 UCrosshairComponent::GetWidth () const
{
    return mCrosshairTexWidth;
}

uint8 UCrosshairComponent::GetHeight () const
{
    return mCrosshairTexHeight;
}

FVector2D UCrosshairComponent::GetScreenCoordinates () const
{
    return mScreenCoordinates;
}

// Called when the game starts.
void UCrosshairComponent::BeginPlay ()
{
    UActorComponent::BeginPlay ();

    if (!mCrosshairTex)
    {
        UE_LOG (LogTemp, Error, TEXT ("CrosshairComponent needs a texture."))

        return;
    }
}
