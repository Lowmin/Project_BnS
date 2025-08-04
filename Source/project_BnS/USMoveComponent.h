#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "USMoveComponent.generated.h"

UENUM(BlueprintType)
enum class EMoveState : uint8
{
	Idle        UMETA(DisplayName = "Idle"),
	Running     UMETA(DisplayName = "Running"),
	Gliding     UMETA(DisplayName = "Gliding"),
	WaterRunning UMETA(DisplayName = "WaterRunning"),
	WallRunning  UMETA(DisplayName = "WallRunning")
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PROJECT_BNS_API USMoveComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USMoveComponent();

protected:
	virtual void BeginPlay() override;

public:
	void SetMoveState(EMoveState NewState);
	void StartSMove();
	void StopSMove();
	void SMoveToggle();
	void StartGlide();
	void StopGlide();
	void GlideToggle();

private:
	EMoveState CurrentMoveState;
	ACharacter* MyPlayer;

	float walkSpeed = 300.f;
	float runSpeed = 600.f;
	float glideSpeed{};
	float glideGravityScale{};
	float glideMinHeight{};
	float CheckGroundDistance();
	float LineTraceStartOffset = -100.f;
	float LineTraceLength = 500.f;
	bool bIsSMove;
	bool bIsGliding;
};