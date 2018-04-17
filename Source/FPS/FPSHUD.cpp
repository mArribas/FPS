// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "FPSHUD.h"

#include "Engine/Texture2D.h"
#include "TextureResource.h"
#include "UObject/ConstructorHelpers.h"
#include "FPS.h"

AFPSHUD::AFPSHUD ()
{
    // Set the crosshair texture
    static ConstructorHelpers::FObjectFinder<UTexture2D> CrosshairTexObj{
        TEXT ("/Game/FirstPerson/Textures/crosshair") };

    mCrosshairTex = CrosshairTexObj.Object;

    if (!mCrosshairTex) return;

    mCrosshairTexWidth  = mCrosshairTex->GetSizeX ();
    mCrosshairTexHeight = mCrosshairTex->GetSizeY ();
}

void AFPSHUD::ChangeCrosshairTexture (
    UTexture2D* const texture,
    const uint8       width,
    const uint8       height)
{
    if (!texture) return;

    mCrosshairTex       = texture;
    mCrosshairTexWidth  = width;
    mCrosshairTexHeight = height;
}

void AFPSHUD::DrawHUD (void)
{
    Super::DrawHUD ();

    if (!mCrosshairTex || !GEngine) return;

    // Viewport Size.
    const FVector2D ViewportSize  {
        GEngine->GameViewport->Viewport->GetSizeXY () };
    // Viewport Center.
    const FVector2D ViewportCenter{
        ViewportSize.X * 0.5f
        , ViewportSize.Y * 0.5f };

    DrawTexture (
        mCrosshairTex
        , (ViewportSize.X * 0.5f) - (mCrosshairTexWidth * 0.5f)
        , (ViewportSize.Y * 0.5f) - (mCrosshairTexHeight * 0.5f)
        , mCrosshairTexWidth * 1.f
        , mCrosshairTexHeight * 1.f
        , 0.f
        , 0.f
        , 1.f
        , 1.f);
}
