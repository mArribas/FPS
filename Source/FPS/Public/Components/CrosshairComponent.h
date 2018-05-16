#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CrosshairComponent.generated.h"

class UTexture2D;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class FPS_API UCrosshairComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    /** Sets default values for the CrosshairComponent's properties. */
    UCrosshairComponent ();

    /** Sets the pointer to the crosshair texture, as well as its width/height. */
    UFUNCTION (BlueprintCallable)
    void SetCrosshairTexture  (
        UTexture2D* Texture,
        uint8       Width,
        uint8       Height);
    /** Sets the crosshair screen coordinates. */
    void SetScreenCoordinates (const FVector2D ScreenCoordinates);

    /** Returns the pointer to the crosshair texture. */
    UTexture2D* GetTexture           () const;
    /** Returns the width of the crosshair. */
    uint8       GetWidth             () const;
    /** Returns the height of the crosshair. */
    uint8       GetHeight            () const;
    /** Returns the screen coordinates of the crosshair. */
    FVector2D   GetScreenCoordinates () const;
protected:
    /** Called when the game starts. */
    void BeginPlay () override;
private:
    /** Pointer to the texture of the crosshair. */
    UPROPERTY (
        EditDefaultsOnly,
        Category = "FPS",
        meta = (DisplayName = "CrosshairTex"))
    UTexture2D* mCrosshairTex;
    /** Width used to draw the texture of the crosshair. */
    UPROPERTY (
        EditDefaultsOnly,
        Category = "FPS",
        meta = (DisplayName = "CrosshairTexWidth"))
    uint8       mCrosshairTexWidth;
    /** Height used to draw the texture of the crosshair. */
    UPROPERTY (
        EditDefaultsOnly,
        Category = "FPS",
        meta = (DisplayName = "CrosshairTexHeight"))
    uint8       mCrosshairTexHeight;
    /** Coordinates used to draw the texture of the crosshair. */
    FVector2D   mScreenCoordinates;
};
