// USMoveComponent.cpp
#include "USMoveComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

USMoveComponent::USMoveComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	bIsSMove = false;
	bIsGliding = false;
	CurrentMoveState = EMoveState::Idle;
	GlideSpeed = 1000.0f;
	WallRunDuration = 5.0f;
	WaterRunCheckRadius = 100.0f;
}

void USMoveComponent::BeginPlay()
{
	Super::BeginPlay();
}

void USMoveComponent::SetSMoveState(EMoveState NewState)
{
	// 상태가 변경되었을 때만 로직 실행
	if (CurrentMoveState != NewState)
	{
		CurrentMoveState = NewState;
		// 델리게이트 호출
		OnSMoveStateChanged.Broadcast(NewState);

		// 상태에 따른 추가 로직 (예: 애니메이션 변경, 효과음 재생 등)
	}
}

void USMoveComponent::StartSMove()
{
	if (!bIsSMove)
	{
		bIsSMove = true;
		SetSMoveState(EMoveState::Running);
	}
}

void USMoveComponent::StopSMove()
{
	if (bIsSMove)
	{
		bIsSMove = false;
		SetSMoveState(EMoveState::Idle);
	}
}

void USMoveComponent::StartGlide()
{
	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (OwnerCharacter && !bIsGliding)
	{
		// 글라이딩 시작 조건 확인 (예: 공중에 있을 때)
		if (OwnerCharacter->GetCharacterMovement()->IsFalling())
		{
			bIsGliding = true;
			SetSMoveState(EMoveState::Gliding);
			// 캐릭터의 이동 속도 및 중력 스케일 변경
			OwnerCharacter->GetCharacterMovement()->AirControl = 1.0f;
			OwnerCharacter->GetCharacterMovement()->GravityScale = 0.5f;
		}
	}
}

void USMoveComponent::StopGlide()
{
	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (OwnerCharacter && bIsGliding)
	{
		bIsGliding = false;
		// 글라이딩 종료
		SetSMoveState(EMoveState::Idle);
		// 원래 캐릭터 이동 속도 및 중력 스케일로 복구
		OwnerCharacter->GetCharacterMovement()->AirControl = 0.2f;
		OwnerCharacter->GetCharacterMovement()->GravityScale = 1.0f;
	}
}

bool USMoveComponent::CheckForWall()
{
	// 벽 감지 로직 (예: 라인 트레이스 사용)
	// ...
	return false;
}

void USMoveComponent::StartWallRun()
{
	if (CheckForWall())
	{
		SetSMoveState(EMoveState::WallRunning);
		// 벽타기 로직
		// ...
	}
}

void USMoveComponent::StopWallRun()
{
	SetSMoveState(EMoveState::Idle);
}

bool USMoveComponent::CheckForWater()
{
	// 물 감지 로직 (예: 구체 충돌 감지 사용)
	// ...
	return false;
}

void USMoveComponent::StartWaterRun()
{
	if (CheckForWater())
	{
		SetSMoveState(EMoveState::WaterRunning);
		// 물 위 달리기 로직
		// ...
	}
}

void USMoveComponent::StopWaterRun()
{
	SetSMoveState(EMoveState::Idle);
}