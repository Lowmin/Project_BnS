// myplayer.cpp
#include "MyPlayer.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "GameFramework/Controller.h"
#include "GameFramework/CharacterMovementComponent.h"

AMyPlayer::AMyPlayer()
{
    PrimaryActorTick.bCanEverTick = true;

    MovementSystem = CreateDefaultSubobject<USMoveComponent>(TEXT("MovementSystem"));

    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength = 300.0f;
    CameraBoom->bUsePawnControlRotation = true;

    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    FollowCamera->bUsePawnControlRotation = false;
}

void AMyPlayer::BeginPlay()
{
    Super::BeginPlay();
}

void AMyPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    UEnhancedInputComponent* EnhancedPlayerInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);

    if (EnhancedPlayerInputComponent != nullptr)
    {
        APlayerController* PlayerController = Cast<APlayerController>(GetController());
        if (PlayerController != nullptr)
        {
            UEnhancedInputLocalPlayerSubsystem* EnhancedSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
            if (EnhancedSubsystem != nullptr)
            {
                EnhancedSubsystem->AddMappingContext(IC_Player, 1);
            }
        }

        EnhancedPlayerInputComponent->BindAction(IA_Movement, ETriggerEvent::Triggered, this, &AMyPlayer::Move);
        EnhancedPlayerInputComponent->BindAction(IA_Jump, ETriggerEvent::Started, this, &AMyPlayer::Jump);
        EnhancedPlayerInputComponent->BindAction(IA_Jump, ETriggerEvent::Completed, this, &AMyPlayer::StopJumping);
        EnhancedPlayerInputComponent->BindAction(IA_Look, ETriggerEvent::Triggered, this, &AMyPlayer::Look);

        EnhancedPlayerInputComponent->BindAction(IA_Run, ETriggerEvent::Started, this, &AMyPlayer::SMoveToggle);
        EnhancedPlayerInputComponent->BindAction(IA_Glide, ETriggerEvent::Started, this, &AMyPlayer::SGlidingToggle);
    }
}

void AMyPlayer::Move(const FInputActionValue& Value)
{
    const FVector2D MovementVector = Value.Get<FVector2D>();

    if (Controller != nullptr)
    {
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);

        const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
        const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

        AddMovementInput(ForwardDirection, MovementVector.Y);
        AddMovementInput(RightDirection, MovementVector.X);
    }
}

void AMyPlayer::Look(const FInputActionValue& Value)
{
    const FVector2D LookAxisVector = Value.Get<FVector2D>();

    if (Controller != nullptr)
    {
        AddControllerYawInput(LookAxisVector.X);
        AddControllerPitchInput(LookAxisVector.Y);
    }
}

void AMyPlayer::SMoveToggle()
{
    if (MovementSystem)
    {
        MovementSystem->SMoveToggle();
    }
}

void AMyPlayer::SGlidingToggle()
{
    if (MovementSystem)
    {
        MovementSystem->GlideToggle();
    }
}