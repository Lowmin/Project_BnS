// USMoveComponent.cpp
#include "USMoveComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

USMoveComponent::USMoveComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	bIsWallClimbing = false;
	WallClimbSpeed = 200.f;
	WallCheckDistance = 100.f;
}

void USMoveComponent::BeginPlay()
{
	Super::BeginPlay();
}

bool USMoveComponent::CanWallClimb() const
{
	// 벽 감지 로직
	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (!OwnerCharacter) return false;

	FHitResult HitResult;
	FVector Start = OwnerCharacter->GetActorLocation();
	FVector End = Start + OwnerCharacter->GetActorForwardVector() * WallCheckDistance;

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(OwnerCharacter);

	if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params))
	{
		// 충돌한 물체가 벽인지 확인 (예: 충돌 채널, 태그 등)
		// 여기서는 간단히 충돌이 발생하면 벽으로 가정
		return true;
	}
	return false;
}

void USMoveComponent::StartWallClimb()
{
	if (bIsWallClimbing) return;

	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (!OwnerCharacter) return;

	// 벽타기 시작
	bIsWallClimbing = true;

	// 캐릭터의 움직임 모드를 비행 모드로 변경
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(MOVE_Flying);

	// 캐릭터의 Z축 속도를 벽타기 속도로 설정
	OwnerCharacter->GetCharacterMovement()->Velocity.Z = WallClimbSpeed;
}

void USMoveComponent::StopWallClimb()
{
	if (!bIsWallClimbing) return;

	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (!OwnerCharacter) return;

	// 벽타기 중지
	bIsWallClimbing = false;

	// 캐릭터의 움직임 모드를 다시 걷기 모드로 변경
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
}