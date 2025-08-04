// USMoveComponent.cpp
#include "USMoveComponent.h"
#include "Engine/Engine.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

USMoveComponent::USMoveComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	bIsSMove = false;
	glideSpeed = 700.f;
	glideGravityScale = 0.1f;
	glideMinHeight = 0.0f;
}

void USMoveComponent::BeginPlay()
{
	MyPlayer = Cast<ACharacter>(GetOwner());

	if (MyPlayer && MyPlayer->GetCharacterMovement())
	{
		MyPlayer->GetCharacterMovement()->MaxWalkSpeed = walkSpeed;
	}
}

void USMoveComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!MyPlayer || !MyPlayer->GetCharacterMovement())
	{
		return;
	}

	if (bIsGliding)
	{
		if (CheckGroundDistance() <= 0.0f)
		{
			StopGlide();
		}
		else
		{
			MyPlayer->GetCharacterMovement()->AddInputVector(FVector(0, 0, -1) * 25.f * DeltaTime);
		}
	}
}

void USMoveComponent::SetMoveState(EMoveState NewState)
{
	if (CurrentMoveState != NewState)
	{
		CurrentMoveState = NewState;
	}
}

void USMoveComponent::StartSMove()
{
	if (!MyPlayer || !MyPlayer->GetCharacterMovement()) return;

	bIsSMove = true;
	SetMoveState(EMoveState::Running);
	MyPlayer->GetCharacterMovement()->MaxWalkSpeed = runSpeed;
}

void USMoveComponent::StopSMove()
{
	if (!MyPlayer || !MyPlayer->GetCharacterMovement()) return;

	bIsSMove = false;
	SetMoveState(EMoveState::Idle);
	MyPlayer->GetCharacterMovement()->MaxWalkSpeed = walkSpeed;
}

void USMoveComponent::SMoveToggle()
{
	if (!MyPlayer) return;

	if (!bIsSMove)
	{
		StartSMove();
	}
	else
	{
		StopSMove();
	}
}

void USMoveComponent::StartGlide()
{
	if (!MyPlayer || !MyPlayer->GetCharacterMovement()) return;

	float groundDistance = CheckGroundDistance();

	if (MyPlayer->GetCharacterMovement()->IsFalling() && groundDistance >= glideMinHeight)
	{
		bIsGliding = true;
		SetMoveState(EMoveState::Gliding);
		MyPlayer->GetCharacterMovement()->SetMovementMode(MOVE_Flying);
		MyPlayer->GetCharacterMovement()->MaxFlySpeed = glideSpeed;
	}
}

void USMoveComponent::StopGlide()
{
	if (!MyPlayer || !MyPlayer->GetCharacterMovement()) return;

	bIsGliding = false;
	SetMoveState(EMoveState::Idle);
	MyPlayer->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
}

void USMoveComponent::GlideToggle()
{
	if (!MyPlayer) return;

	if (!bIsGliding)
	{
		StartGlide();
	}
	else
	{
		StopGlide();
	}
}

float USMoveComponent::CheckGroundDistance()
{
	FHitResult HitResult;
	FVector StartLocation = MyPlayer->GetActorLocation();
	FVector EndLocation = StartLocation - FVector(0, 0, LineTraceLength);

	StartLocation.Z += LineTraceStartOffset;

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(MyPlayer);

	bool bHasHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		StartLocation,
		EndLocation,
		ECollisionChannel::ECC_Visibility,
		QueryParams
	);

	if (bHasHit)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("Ground Distance: %f"), HitResult.Distance));
		return HitResult.Distance;
	}

	return -1.0f;
}
