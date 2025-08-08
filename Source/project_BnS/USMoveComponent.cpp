#include "USMoveComponent.h"
#include "Engine/Engine.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "DrawDebugHelpers.h"

USMoveComponent::USMoveComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	CurrentMoveState = EMoveState::Idle;
}

void USMoveComponent::BeginPlay()
{
	Super::BeginPlay();
	MyPlayer = Cast<ACharacter>(GetOwner());
	if (MyPlayer && MyPlayer->GetCharacterMovement())
	{
		MyPlayer->GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	}
}

void USMoveComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!MyPlayer || !MyPlayer->GetCharacterMovement())
	{
		return;
	}

	FVector curVelocity = MyPlayer->GetCharacterMovement()->Velocity;

	if (CurrentMoveState == EMoveState::Running && MyPlayer->GetCharacterMovement()->IsMovingOnGround())
	{
		FHitResult WallHit;
		if (CheckWall(WallHit))
		{
			StartWallRun(WallHit);
		}
	}

	if (CurrentMoveState == EMoveState::Running && curVelocity.Size() <= 0)
	{
		SetMoveState(EMoveState::Idle);
	}

	if (CurrentMoveState == EMoveState::WallRunning)
	{
		WallRun();
	}

	if (CurrentMoveState == EMoveState::Gliding)
	{
		Glide(DeltaTime);
	}

	if (GEngine)
	{
		FString StateString = UEnum::GetValueAsString(CurrentMoveState);
		GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Yellow, FString::Printf(TEXT("MoveState: %s"), *StateString));
	}
}

void USMoveComponent::SetMoveState(EMoveState NewState)
{
	if (CurrentMoveState != NewState)
	{
		CurrentMoveState = NewState;
		SetMovementSpeed(NewState);
	}
}

EMoveState USMoveComponent::GetMoveState() const
{
	return CurrentMoveState;
}

void USMoveComponent::SetMovementSpeed(EMoveState NewState)
{
	if (!MyPlayer || !MyPlayer->GetCharacterMovement()) return;

	switch (NewState)
	{
	case EMoveState::Idle:
		MyPlayer->GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
		break;
	case EMoveState::Running:
		MyPlayer->GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
		break;
	case EMoveState::WallRunning:
		break;
	case EMoveState::Gliding:
		MyPlayer->GetCharacterMovement()->MaxFlySpeed = GlideSpeed;
		break;
	default:
		break;
	}
}

bool USMoveComponent::CheckWall(FHitResult& OutHit)
{
	if (!MyPlayer || !GetWorld()) return false;

	FVector Start = MyPlayer->GetActorLocation();
	FVector End = Start + MyPlayer->GetActorForwardVector() * WallCheckDistance;

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(MyPlayer);

	bool bHasHit = GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, QueryParams);

	DrawDebugLine(
		GetWorld(),
		Start,
		End,
		bHasHit ? FColor::Green : FColor::Red,
		false,
		1.0f,
		0,
		1.0f
	);

	if (bHasHit)
	{
		if (OutHit.GetActor() && OutHit.GetActor()->ActorHasTag("Wall"))
		{
			return true;
		}
	}

	return false;
}

void USMoveComponent::StartWallRun(const FHitResult& WallHit)
{
	if (!MyPlayer || !MyPlayer->GetCharacterMovement()) return;

	SetMoveState(EMoveState::WallRunning);
	MyPlayer->GetCharacterMovement()->SetMovementMode(MOVE_Flying);
	WallRunNormal = WallHit.ImpactNormal;
}

void USMoveComponent::StopWallRun()
{
	if (!MyPlayer || !MyPlayer->GetCharacterMovement()) return;

	SetMoveState(EMoveState::Idle);
	MyPlayer->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
}

void USMoveComponent::WallRun()
{
	if (!MyPlayer || !MyPlayer->GetCharacterMovement()) return;

	FHitResult WallHit;
	if (CheckWall(WallHit))
	{
		MyPlayer->GetCharacterMovement()->Velocity = FVector::UpVector * WallRunSpeed;
	}
	else
	{
		StopWallRun();
	}
}

void USMoveComponent::SMoveToggle()
{
	if (!MyPlayer) return;
	if (CurrentMoveState == EMoveState::Running)
	{
		StopSMove();
	}
	else if (CurrentMoveState == EMoveState::Idle)
	{
		StartSMove();
	}
}

void USMoveComponent::StartSMove()
{
	if (!MyPlayer || !MyPlayer->GetCharacterMovement()) return;

	if (CurrentMoveState == EMoveState::Gliding)
	{
		StopGlide();
	}
	SetMoveState(EMoveState::Running);
}

void USMoveComponent::StopSMove()
{
	if (!MyPlayer || !MyPlayer->GetCharacterMovement()) return;

	SetMoveState(EMoveState::Idle);
}

void USMoveComponent::StartGlide()
{
	if (!MyPlayer || !MyPlayer->GetCharacterMovement()) return;

	if (CurrentMoveState == EMoveState::Running)
	{
		StopSMove();
	}
	float groundDistance = CheckGroundDistance();
	if (MyPlayer->GetCharacterMovement()->IsFalling() && groundDistance >= GlideMinHeight)
	{
		SetMoveState(EMoveState::Gliding);
		MyPlayer->GetCharacterMovement()->SetMovementMode(MOVE_Flying);
		MyPlayer->GetCharacterMovement()->GravityScale = GlideGravityScale;
	}
}

void USMoveComponent::StopGlide()
{
	if (!MyPlayer || !MyPlayer->GetCharacterMovement()) return;

	SetMoveState(EMoveState::Running);
	MyPlayer->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	MyPlayer->GetCharacterMovement()->GravityScale = 1.0f;
}

void USMoveComponent::GlideToggle()
{
	if (!MyPlayer) return;

	if (CurrentMoveState != EMoveState::Gliding)
	{
		StartGlide();
	}
	else
	{
		StopGlide();
	}
}
void USMoveComponent::Glide(float DeltaTime)
{
	if (CurrentMoveState != EMoveState::Gliding) return;

	if (MyPlayer->GetCharacterMovement()->IsMovingOnGround())
	{
		StopGlide();
		return;
	}

	MyPlayer->GetCharacterMovement()->Velocity.Z = -GlideDescentSpeed;
}

void USMoveComponent::SJump()
{
	if (!MyPlayer || !MyPlayer->GetCharacterMovement()) return;

	if (CurrentMoveState == EMoveState::WallRunning)
	{
		FVector LaunchVelocity = WallRunNormal * JumpVelocity + FVector(0.f, 0.f, JumpVelocity);
		MyPlayer->LaunchCharacter(LaunchVelocity, false, true);
		StopWallRun();
		return;
	}

	if (MyPlayer->GetCharacterMovement()->IsFalling())
	{
		return;
	}

	float CurrentJumpVelocity = JumpVelocity;

	if (CurrentMoveState == EMoveState::Running)
	{
		CurrentJumpVelocity = JumpVelocity * 2.f;
	}

	if (CurrentMoveState == EMoveState::Gliding)
	{
		StopGlide();
		return;
	}

	MyPlayer->LaunchCharacter(FVector(0.f, 0.f, CurrentJumpVelocity), false, true);
}

float USMoveComponent::CheckGroundDistance()
{
	if (!MyPlayer || !GetWorld()) return -1.0f;

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
		return HitResult.Distance;
	}
	return -1.0f;
}
