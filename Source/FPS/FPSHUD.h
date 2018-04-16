// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "FPSHUD.generated.h"

class UTexture2D;

UCLASS()
class AFPSHUD : public AHUD
{
    GENERATED_BODY()

public:
    AFPSHUD ();

    /** Primary draw call for the HUD */
    virtual void DrawHUD (void) override;
protected:
    /** Crosshair asset pointer */
    UPROPERTY (EditAnywhere, BlueprintReadOnly, Category = "FPS|HUD")
        UTexture2D* mCrosshairTex;
};
