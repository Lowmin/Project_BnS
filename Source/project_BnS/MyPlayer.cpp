// Fill out your copyright notice in the Description page of Project Settings.

#include "MyPlayer.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/Controller.h"

AMyPlayer::AMyPlayer()
{
	PrimaryActorTick.bCanEverTick = true;
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
	}
}

void AMyPlayer::Move(const FInputActionValue& Value)
{
	FVector2D InputValue = Value.Get<FVector2D>();

	if (Controller != nullptr && (InputValue.X != 0.0f || InputValue.Y != 0.0f))
	{
		const FRotator YawRotation(0, Controller->GetControlRotation().Yaw, 0);

		if (InputValue.X != 0.0f)
		{
			const FVector RightDirection = UKismetMathLibrary::GetRightVector(YawRotation);
			AddMovementInput(RightDirection, InputValue.X);
		}

		if (InputValue.Y != 0.0f)
		{
			const FVector ForwardDirection = YawRotation.Vector();
			AddMovementInput(ForwardDirection, InputValue.Y);
		}
	}
}
