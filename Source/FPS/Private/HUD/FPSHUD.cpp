#include "FPSHUD.h"

#include "Public/Components/CrosshairComponent.h"

AFPSHUD::AFPSHUD ()
{
    mCrosshairComponent = nullptr;
}

void AFPSHUD::SetCrosshair (UCrosshairComponent* const Crosshair)
{
    mCrosshairComponent = Crosshair;
}

void AFPSHUD::DrawHUD ()
{
    AHUD::DrawHUD ();

    // Draw crosshair.
    if (!mCrosshairComponent) return;

    // Get crosshair properties.
    FVector2D ScreenCoordinates{ mCrosshairComponent->GetScreenCoordinates () };
    float     Width            { mCrosshairComponent->GetWidth () * 1.f };
    float     Height           { mCrosshairComponent->GetHeight () * 1.f };

    // Draw the crosshair.
    DrawTexture (
        Cast<UTexture>(mCrosshairComponent->GetTexture ())
        , ScreenCoordinates.X - (Width * 0.5)
        , ScreenCoordinates.Y - (Height * 0.5)
        , Width
        , Height
        , 0.f
        , 0.f
        , 1.f
        , 1.f);
}
