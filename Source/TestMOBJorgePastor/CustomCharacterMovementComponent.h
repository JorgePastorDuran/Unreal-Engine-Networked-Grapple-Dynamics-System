#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CustomCharacterMovementComponent.generated.h"


UCLASS()
class TESTMOBJORGEPASTOR_API UCustomCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:

	UPROPERTY()
	class ACustomPlayer* CustomPlayer;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(Server, Reliable)
	void TryGrapple(FVector Value);

	UFUNCTION(Server, Reliable)
	void TryDisconnect();

protected:
	virtual void InitializeComponent() override;
	virtual void PhysCustom(float deltaTime, int32 Iterations) override;

private:
	UPROPERTY(Replicated)
	FVector GrappleTargetLocation;

	UPROPERTY(Replicated)
	float TargetDistance;

	void GrappleToSurface(float deltaTime, int32 Iterations);
	void ExitGrapple();
};

UENUM(BlueprintType)
enum ECustomMovementMode
{
	CMOVE_None			UMETA(Hidden),
	CMOVE_Grapple		UMETA(DisplayName = "Grapple"),
	CMOVE_Hanging		UMETA(DisplayName = "Hanging"),
	CMOVE_MAX			UMETA(Hidden),
};