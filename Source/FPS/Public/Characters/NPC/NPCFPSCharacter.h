#pragma once

#include "CoreMinimal.h"
#include "FPSCharacter.h"
#include "NPCFPSCharacter.generated.h"

class UBehaviorTree;

/** Basic NPC character. */
UCLASS()
class FPS_API ANPCFPSCharacter : public AFPSCharacter
{
    GENERATED_BODY()

public:
    /** Sets default values for the ANPCFPSCharacter's properties. */
    ANPCFPSCharacter ();

    /** Called every frame. */
    void Tick (float DeltaTime) override;
protected:
    /** Called when the game starts or when spawned. */
    void BeginPlay () override;

    /** Behavior tree. */
    UPROPERTY (
        EditAnywhere,
        Category = "FPS",
        meta = (DisplayName = "NPCBehavior"))
    UBehaviorTree* mNPCBehavior;
};
