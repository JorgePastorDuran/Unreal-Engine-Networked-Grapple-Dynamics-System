#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CustomPlayer.generated.h"

UCLASS()
class TESTMOBJORGEPASTOR_API ACustomPlayer : public ACharacter
{
    GENERATED_BODY()

public:
    ACustomPlayer(const class FObjectInitializer& ObjectInitializer);

    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
    void MoveInAxisX(float Value);
    void MoveInAxisY(float Value);
    void RotateYaw(float Value);
    void RotatePitch(float Value);
    void Grappled();


private:
    void MoveInDirection(float Value, EAxis::Type Axis);

    void RotateController(float Value, bool bIsYaw);

    UPROPERTY(EditDefaultsOnly, Category = "Movement")
    class UCustomCharacterMovementComponent* CustomCharacterMovementComponent;

    UPROPERTY(VisibleAnywhere, Category = "Components")
    UStaticMeshComponent* BaseMesh;

    UPROPERTY(VisibleAnywhere)
    class UCameraComponent* PlayerCamera;
};
