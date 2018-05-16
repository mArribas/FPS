#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "FPSHUD.generated.h"

class UCrosshairComponent;

/** Basic HUD. */
UCLASS()
class AFPSHUD : public AHUD
{
    GENERATED_BODY()

public:
    AFPSHUD ();

    /** Sets the new UCrosshairComponent to point at. */
    void SetCrosshair (UCrosshairComponent* const Crosshair);

    /** Primary draw call for the HUD */
    void DrawHUD () override;
private:
    /** Pointer to the UCrosshairComponent to use. */
    UCrosshairComponent* mCrosshairComponent;
};
