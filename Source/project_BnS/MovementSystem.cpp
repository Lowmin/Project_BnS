#include "MovementSystem.h"
#include "GameFramework/Controller.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"

AMovementSystem::AMovementSystem()
{

}

void AMovementSystem::BeginPlay()
{
	Super::BeginPlay();
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	}
}

void AMovementSystem::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		const FRotator YawRotation(0, Controller->GetControlRotation().Yaw, 0);
		const FVector ForwardDirection = YawRotation.Vector();
		AddMovementInput(ForwardDirection, Value);
	}
}

void AMovementSystem::MoveRight(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		const FRotator YawRotation(0, Controller->GetControlRotation().Yaw, 0);
		const FVector RightDirection = UKismetMathLibrary::GetRightVector(YawRotation);
		AddMovementInput(RightDirection, Value);
	}
}

void AMovementSystem::Look(float YawValue, float PitchValue)
{
	if (YawValue != 0.0f)
	{
		AddControllerYawInput(YawValue);
	}
	if (PitchValue != 0.0f)
	{
		AddControllerPitchInput(PitchValue);
	}
}

void AMovementSystem::BeginRunning()
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = 300.0f;
	}
}

void AMovementSystem::StopRunning()
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	}
}