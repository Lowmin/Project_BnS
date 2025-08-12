#include "USMoveComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/Engine.h"
#include "GameFramework/PlayerController.h"
#include "LatentActions.h"

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

	if (!MyPlayer) return;

	if (CurrentMoveState == EMoveState::Running)
	{
		FHitResult WallHit;
		if (CanWallRun(WallHit))
		{
			BeginWallRun(WallHit);
		}
	}

	if (CurrentMoveState == EMoveState::Running && MyPlayer->GetVelocity().SizeSquared() < 1.0f)
	{
		SetMoveState(EMoveState::Idle);
	}

	else if (CurrentMoveState == EMoveState::WallRunning)
	{
		TickWallRun();
	}
	else if (CurrentMoveState == EMoveState::Gliding || CurrentMoveState == EMoveState::FastGliding)
	{
		Glide(DeltaTime);
	}

	TickMeshTilt(DeltaTime);

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

	if (NewState == EMoveState::WallRunning || NewState == EMoveState::ClimbingLedge) return;

	switch (NewState)
	{
	case EMoveState::Idle:
		MyPlayer->GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
		break;
	case EMoveState::Running:
		MyPlayer->GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
		break;
	case EMoveState::Gliding:
		MyPlayer->GetCharacterMovement()->MaxFlySpeed = GlideSpeed;
		break;
	case EMoveState::FastGliding:
		MyPlayer->GetCharacterMovement()->MaxFlySpeed = FastGlideSpeed;
		break;
	default:
		break;
	}
}

void USMoveComponent::SMoveToggle()
{
	if (!MyPlayer) return;

	switch (CurrentMoveState)
	{
	case EMoveState::Idle:
		StartSMove();
		break;

	case EMoveState::Running:
		StopSMove();
		break;

	case EMoveState::Gliding:
		SetMoveState(EMoveState::FastGliding);
		break;

	case EMoveState::FastGliding:
		SetMoveState(EMoveState::Gliding);
		break;

	default:
		break;
	}
}

void USMoveComponent::StartSMove()
{
	if (CurrentMoveState == EMoveState::Gliding) StopGlide();
	SetMoveState(EMoveState::Running);
}

void USMoveComponent::StopSMove()
{
	SetMoveState(EMoveState::Idle);
}

void USMoveComponent::StartGlide()
{
	if (!MyPlayer || !MyPlayer->GetCharacterMovement()) return;

	if (MyPlayer->GetCharacterMovement()->IsFalling() && CheckGroundDistance() >= GlideMinHeight)
	{
		if (CurrentMoveState == EMoveState::Running) StopSMove();
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
	if (CurrentMoveState == EMoveState::Gliding || CurrentMoveState == EMoveState::FastGliding)
	{
		StopGlide();
	}
	else
	{
		StartGlide();
	}
}

void USMoveComponent::Glide(float DeltaTime)
{
	if (!MyPlayer || !MyPlayer->GetCharacterMovement()) return;

	GroundDistance = CheckGroundDistance();

	if (GroundDistance <= 0.1f)
	{
		StopGlide();
		return;
	}
	MyPlayer->GetCharacterMovement()->Velocity.Z = -GlideDescentSpeed;
}

void USMoveComponent::FastGlideToggle()
{
	if (CurrentMoveState != EMoveState::FastGliding)
	{
		SetMoveState(EMoveState::FastGliding);
	}
	else
	{
		SetMoveState(EMoveState::Gliding);
	}
}

void USMoveComponent::SJump()
{
	if (!MyPlayer || !MyPlayer->GetCharacterMovement() || MyPlayer->GetCharacterMovement()->IsFalling())
	{
		return;
	}

	const float CurrentJumpVelocity = (CurrentMoveState == EMoveState::Running) ? JumpVelocity * 2.f : JumpVelocity;
	MyPlayer->LaunchCharacter(FVector(0.f, 0.f, CurrentJumpVelocity), false, true);
}

void USMoveComponent::WallJump()
{
	if (!MyPlayer || CurrentMoveState != EMoveState::WallRunning) return;
	EndWallRun();

	const FVector JumpDirection = (WallNormal + FVector(0.f, 0.f, 0.7f)).GetSafeNormal();

	MyPlayer->LaunchCharacter(JumpDirection * WallJumpForce, true, true);
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

	if (GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, QueryParams))
	{
		return HitResult.Distance;
	}
	return -1.0f;
}

bool USMoveComponent::CanWallRun(FHitResult& OutHit)
{
	if (!MyPlayer || !MyPlayer->GetCharacterMovement()) return false;
	if (MyPlayer->GetCharacterMovement()->IsFalling()) return false;
	if (MyPlayer->GetVelocity().SizeSquared() < 1.0f) return false;

	return CheckWall(OutHit);
}

bool USMoveComponent::CheckWall(FHitResult& OutHit)
{
	if (!MyPlayer) return false;

	const FVector Start = MyPlayer->GetActorLocation();
	const FVector End = Start + MyPlayer->GetActorForwardVector() * WallTraceSettings.MaxReach;

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(MyPlayer);

	UKismetSystemLibrary::SphereTraceSingle(GetWorld(), Start, End, WallTraceSettings.TraceRadius,
		UEngineTypes::ConvertToTraceType(ECC_Visibility), false, ActorsToIgnore,
		EDrawDebugTrace::None, OutHit, true);

	return OutHit.bBlockingHit && OutHit.GetActor() && OutHit.GetActor()->ActorHasTag("Wall");
}

void USMoveComponent::BeginWallRun(const FHitResult& WallHit)
{
	if (!MyPlayer || !MyPlayer->GetCharacterMovement()) return;

	SetMoveState(EMoveState::WallRunning);
	WallNormal = WallHit.ImpactNormal;

	bOriginalOrientRotationToMovement = MyPlayer->GetCharacterMovement()->bOrientRotationToMovement;
	bOriginalUseControllerRotationYaw = MyPlayer->bUseControllerRotationYaw;

	MyPlayer->GetCharacterMovement()->bOrientRotationToMovement = false;
	MyPlayer->bUseControllerRotationYaw = false;

	const FRotator LookAtWallRotation = (-WallNormal).Rotation();
	MyPlayer->SetActorRotation(FRotator(0.0f, LookAtWallRotation.Yaw, 0.0f));

	MyPlayer->GetCharacterMovement()->SetMovementMode(MOVE_Flying);

	TargetMeshPitch = -60.0f;
}

void USMoveComponent::EndWallRun()
{
	if (!MyPlayer || !MyPlayer->GetCharacterMovement()) return;

	TargetMeshPitch = 0.0f;
	SJump();

	MyPlayer->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	MyPlayer->GetCharacterMovement()->bOrientRotationToMovement = bOriginalOrientRotationToMovement;
	MyPlayer->bUseControllerRotationYaw = bOriginalUseControllerRotationYaw;

	SetMoveState(EMoveState::Running);
}

void USMoveComponent::TickWallRun()
{
	if (!MyPlayer || !MyPlayer->GetCharacterMovement()) return;

	FHitResult LedgeHit;
	if (CheckLedge(LedgeHit))
	{
		ClimbLedge(LedgeHit);
		return;
	}

	FHitResult WallHit;
	if (CheckWall(WallHit))
	{
		APlayerController* PlayerController = MyPlayer->GetController<APlayerController>();
		if (PlayerController)
		{
			const FVector UpDirection = FVector::UpVector;

			const FVector StrafeDirection = FVector::CrossProduct(WallNormal, UpDirection);

			const bool bMoveUp = PlayerController->IsInputKeyDown(EKeys::W);
			const bool bMoveLeft = PlayerController->IsInputKeyDown(EKeys::A);
			const bool bMoveRight = PlayerController->IsInputKeyDown(EKeys::D);

			FVector TargetDirection = FVector::ZeroVector;

			if (bMoveUp)
			{
				TargetDirection += UpDirection;
			}
			if (bMoveLeft)
			{
				TargetDirection -= StrafeDirection;
			}
			if (bMoveRight)
			{
				TargetDirection += StrafeDirection;
			}

			const FVector TargetVelocity = TargetDirection.IsNearlyZero() ? FVector::ZeroVector : TargetDirection.GetSafeNormal() * WallRunSpeed;
			const FVector CurrentVelocity = MyPlayer->GetCharacterMovement()->Velocity;
			const float InterpSpeed = 8.0f;
			const FVector NewVelocity = FMath::VInterpTo(CurrentVelocity, TargetVelocity, GetWorld()->GetDeltaSeconds(), InterpSpeed);

			MyPlayer->GetCharacterMovement()->Velocity = NewVelocity;
		}
	}
	else
	{
		EndWallRun();
	}
}

bool USMoveComponent::CheckLedge(FHitResult& OutHit)
{
	if (!MyPlayer) return false;

	const FVector Forward = MyPlayer->GetActorForwardVector();
	const FVector Up = MyPlayer->GetActorUpVector();
	const FVector Start = MyPlayer->GetActorLocation() + Up * MyPlayer->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

	FHitResult ForwardTraceHit;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(MyPlayer);

	UKismetSystemLibrary::SphereTraceSingle(GetWorld(), Start, Start + Forward * LedgeTraceSettings.MaxReach,
		LedgeTraceSettings.TraceRadius, UEngineTypes::ConvertToTraceType(ECC_Visibility), false, ActorsToIgnore,
		EDrawDebugTrace::None, ForwardTraceHit, true);

	if (!ForwardTraceHit.bBlockingHit)
	{
		const FVector DownTraceStart = Start + Forward * LedgeTraceSettings.MaxReach;
		UKismetSystemLibrary::SphereTraceSingle(GetWorld(), DownTraceStart, DownTraceStart - Up * LedgeTraceSettings.MaxReach * 2.0f,
			LedgeTraceSettings.TraceRadius, UEngineTypes::ConvertToTraceType(ECC_Visibility), false, ActorsToIgnore,
			EDrawDebugTrace::None, OutHit, true);

		return OutHit.bBlockingHit;
	}

	return false;
}

void USMoveComponent::ClimbLedge(const FHitResult& LedgeHit)
{
	if (!MyPlayer || !MyPlayer->GetCharacterMovement()) return;

	SetMoveState(EMoveState::ClimbingLedge);

	const FVector TargetLocation = LedgeHit.ImpactPoint + WallNormal * 50.0f + FVector(0, 0, MyPlayer->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() + 5.0f);

	FLatentActionInfo LatentInfo;
	LatentInfo.Linkage = 0;
	LatentInfo.CallbackTarget = this;
	LatentInfo.ExecutionFunction = FName("OnClimbLedgeFinished");
	LatentInfo.UUID = __LINE__;

	UKismetSystemLibrary::MoveComponentTo(MyPlayer->GetCapsuleComponent(), TargetLocation, MyPlayer->GetActorRotation(), true, true, 0.2f, false, EMoveComponentAction::Type::Move, LatentInfo);
}

void USMoveComponent::OnClimbLedgeFinished()
{
	EndWallRun();
}

void USMoveComponent::TickMeshTilt(float DeltaTime)
{
	if (!MyPlayer || !MyPlayer->GetMesh()) return;

	FRotator CurrentRelativeRotation = MyPlayer->GetMesh()->GetRelativeRotation();

	float NewRoll = FMath::FInterpTo(CurrentRelativeRotation.Roll, TargetMeshPitch, DeltaTime, TiltInterpSpeed);
	MyPlayer->GetMesh()->SetRelativeRotation(FRotator(CurrentRelativeRotation.Pitch, CurrentRelativeRotation.Yaw, NewRoll));
}
