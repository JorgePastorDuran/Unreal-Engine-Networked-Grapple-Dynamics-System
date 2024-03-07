#include "CustomPlayer.h"
#include "CustomCharacterMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

ACustomPlayer::ACustomPlayer(const class FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer.SetDefaultSubobjectClass<UCustomCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
    CustomCharacterMovementComponent = Cast<UCustomCharacterMovementComponent>(GetCharacterMovement());
    CustomCharacterMovementComponent->SetIsReplicated(true);
    PrimaryActorTick.bCanEverTick = true;

    PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    PlayerCamera->SetupAttachment(RootComponent);
    PlayerCamera->bUsePawnControlRotation = true;

    BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
    BaseMesh->SetupAttachment(PlayerCamera);
}


void ACustomPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    // Bind axis events
    PlayerInputComponent->BindAxis("BackwardForward", this, &ACustomPlayer::MoveInAxisX);
    PlayerInputComponent->BindAxis("LeftRight", this, &ACustomPlayer::MoveInAxisY);

    // Bind rotation events
    PlayerInputComponent->BindAxis("Yaw", this, &ACustomPlayer::RotateYaw);
    PlayerInputComponent->BindAxis("PitchUp", this, &ACustomPlayer::RotatePitch);

    // Bind action events
    PlayerInputComponent->BindAction("Grappled", IE_Pressed, this, &ACustomPlayer::Grappled);
}

void ACustomPlayer::MoveInAxisX(float Value)
{
    MoveInDirection(Value, EAxis::X);
}

void ACustomPlayer::MoveInAxisY(float Value)
{
    MoveInDirection(Value, EAxis::Y);
}

void ACustomPlayer::MoveInDirection(float Value, EAxis::Type Axis)
{
    if (Value != 0.0f && CustomCharacterMovementComponent->MovementMode != MOVE_Custom)
    {
        FVector ControlForwardVector = Controller->GetControlRotation().Quaternion().GetForwardVector();
        ControlForwardVector.Normalize();

        FVector Direction;
        if (Axis == EAxis::X)
        {
            Direction = ControlForwardVector;
        }
        else if (Axis == EAxis::Y)
        {
            Direction = FVector::CrossProduct(FVector::UpVector, ControlForwardVector);
        }
        AddMovementInput(Direction, Value);
    }
}


void ACustomPlayer::RotateYaw(float Value)
{
    RotateController(Value, true);
}

void ACustomPlayer::RotatePitch(float Value)
{
    RotateController(Value, false);
}

void ACustomPlayer::RotateController(float Value, bool bIsYaw)
{
    if (Value != 0.0f)
    {
        const float RotationRate = 45.f;
        const float Amount = Value * RotationRate * GetWorld()->GetDeltaSeconds();

        if (bIsYaw)
        {
            AddControllerYawInput(Amount);
            return;
        }
        AddControllerPitchInput(Amount);
    }
}


void ACustomPlayer::Grappled()
{
    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
    if (!PlayerController)
    {
        return;
    }

    FVector CameraLocation = PlayerCamera->GetComponentLocation();
    FRotator CameraRotation = PlayerCamera->GetComponentRotation();
    PlayerController->GetPlayerViewPoint(CameraLocation, CameraRotation);
    FVector TraceEnd = CameraLocation + (CameraRotation.Vector() * 10000.f);

    FHitResult HitResult;
    const bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, CameraLocation, TraceEnd, ECC_Visibility);

    if (bHit)
    {
        CustomCharacterMovementComponent->TryGrapple(HitResult.Location);
        return;
    }
    
    CustomCharacterMovementComponent->TryDisconnect();
  
}


