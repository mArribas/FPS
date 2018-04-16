// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "FPSHUD.h"
#include "Engine/Canvas.h"
#include "Engine/Texture2D.h"
#include "TextureResource.h"
#include "CanvasItem.h"
#include "UObject/ConstructorHelpers.h"

AFPSHUD::AFPSHUD ()
{
    // Set the crosshair texture
    static ConstructorHelpers::FObjectFinder<UTexture2D> CrosshairTexObj{
        TEXT ("/Game/FirstPerson/Textures/FirstPersonCrosshair") };

    mCrosshairTex = CrosshairTexObj.Object;
}

void AFPSHUD::DrawHUD (void)
{
    Super::DrawHUD ();

    // Draw very simple crosshair

    // find center of the Canvas
    const FVector2D center (Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);
    // Offset by half the texture's dimensions so that the center of the
    // texture aligns with the center of the Canvas
    const FVector2D crosshairDrawPosition{ (center.X), (center.Y + 20.0f) };
    // draw the crosshair
    FCanvasTileItem tileItem{
        crosshairDrawPosition
        , mCrosshairTex->Resource
        , FLinearColor::White };

    tileItem.BlendMode = SE_BLEND_Translucent;
    Canvas->DrawItem (tileItem);
}
