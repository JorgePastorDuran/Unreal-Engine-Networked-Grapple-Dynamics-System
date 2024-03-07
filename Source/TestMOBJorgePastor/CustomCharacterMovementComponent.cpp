#include "CustomCharacterMovementComponent.h"
#include "CustomPlayer.h"
#include "Components/CapsuleComponent.h"
#include "Net/UnrealNetwork.h"

void UCustomCharacterMovementComponent::InitializeComponent()
{
	Super::InitializeComponent();

	CustomPlayer = Cast<ACustomPlayer>(GetOwner());
	TargetDistance = CustomPlayer->GetCapsuleComponent()->GetUnscaledCapsuleRadius() + 10.f;
}

void UCustomCharacterMovementComponent::PhysCustom(float deltaTime, int32 Iterations)
{
	Super::PhysCustom(deltaTime, Iterations);

    if (CustomMovementMode == CMOVE_Grapple)
    {
        GrappleToSurface(deltaTime, Iterations);
    }
}

void UCustomCharacterMovementComponent::GrappleToSurface(float deltaTime, int32 Iterations)
{
    if (deltaTime < MIN_TICK_TIME)
    {
        return;
    }

    FVector Direction = GrappleTargetLocation - UpdatedComponent->GetComponentLocation();
    FVector DirectionXY = FVector(Direction.X, Direction.Y, 0.f); 

    if (DirectionXY.SizeSquared() < FMath::Square(TargetDistance))
    {
        ExitGrapple();
        StartNewPhysics(deltaTime, Iterations);
        return;
    }

    CalcVelocity(deltaTime, 0.f, false, GetMaxBrakingDeceleration());
    Iterations++;
    FHitResult Hit(1.f);
    FVector Adjusted = Velocity * deltaTime;
    FQuat NewRotation = FRotationMatrix::MakeFromXZ(UpdatedComponent->GetForwardVector().GetSafeNormal2D(), FVector::UpVector).ToQuat();

    SafeMoveUpdatedComponent(Adjusted, NewRotation, true, Hit);

    if (Hit.Time < 1.f)
    {
        HandleImpact(Hit, deltaTime, Adjusted);
        SlideAlongSurface(Adjusted, (1.f - Hit.Time), Hit.Normal, Hit, true);
    }
    else
    {
        Velocity = (GrappleTargetLocation - UpdatedComponent->GetComponentLocation()).GetSafeNormal() * 700.f;
    }

    Direction = GrappleTargetLocation - UpdatedComponent->GetComponentLocation();
    if (Direction.SizeSquared() < FMath::Square(TargetDistance))
    {
        ExitGrapple();
    }
}


void UCustomCharacterMovementComponent::ExitGrapple()
{
	FQuat NewRotation = FRotationMatrix::MakeFromXZ(UpdatedComponent->GetForwardVector().GetSafeNormal2D(), FVector::UpVector).ToQuat();
	FHitResult Hit;
	SafeMoveUpdatedComponent(FVector::ZeroVector, NewRotation, true, Hit);
	SetMovementMode(MOVE_Custom, CMOVE_Hanging);
}

void UCustomCharacterMovementComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCustomCharacterMovementComponent, GrappleTargetLocation);
	DOREPLIFETIME(UCustomCharacterMovementComponent, TargetDistance);
}

void UCustomCharacterMovementComponent::TryGrapple_Implementation(FVector GrappleLocation)
{
    if (MovementMode != EMovementMode::MOVE_Custom && CustomMovementMode != CMOVE_Grapple)
    {
        GrappleTargetLocation = GrappleLocation;
        Velocity = (GrappleTargetLocation - UpdatedComponent->GetComponentLocation()).GetSafeNormal() * 700.f;
        SetMovementMode(MOVE_Custom, CMOVE_Grapple);
        return;
    }
    TryDisconnect();
}

void UCustomCharacterMovementComponent::TryDisconnect_Implementation()
{
    if (MovementMode == EMovementMode::MOVE_Custom && CustomMovementMode == CMOVE_Hanging)
    {
        SetMovementMode(MOVE_Falling);
    }
}