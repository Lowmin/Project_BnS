#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "USMoveComponent.generated.h"

class AMyPlayer;

USTRUCT(BlueprintType)
struct FParkourTraceSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parkour")
	float MaxReach = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parkour")
	float TraceRadius = 20.0f;
};

UENUM(BlueprintType)
enum class EMoveState : uint8
{
	Idle			UMETA(DisplayName = "Idle"),
	Running			UMETA(DisplayName = "Running"),
	Gliding			UMETA(DisplayName = "Gliding"),
	FastGliding		UMETA(DisplayName = "FastGliding"),
	WallRunning		UMETA(DisplayName = "WallRunning"),
	ClimbingLedge	UMETA(DisplayName = "ClimbingLedge"),
	WaterRunning	UMETA(DisplayName = "WaterRunning"),
	Swim			UMETA(DisplayName = "Swim")
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PROJECT_BNS_API USMoveComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USMoveComponent();

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	void SetMoveState(EMoveState NewState);
	EMoveState GetMoveState() const;

	// 움직임
	void StartSMove();
	void StopSMove();
	void SMoveToggle();
	void TickRunning();


	// 글라이드
	void StartGlide();
	void StopGlide();
	void GlideToggle();
	void Glide(float DeltaTime);
	void FastGlideToggle();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SMove|Glide")
	float GroundDistance{};

	// 점프
	void SJump();
	void WallJump();
	void JumpOnWater();

private:
	void SetMovementSpeed(EMoveState NewState);
	float CheckGroundDistance();
	void UsedStamina(float DeltaTime);

	// 벽 체크
	bool CanWallRun(FHitResult& OutHit);
	bool CheckWall(FHitResult& OutHit);
	void BeginWallRun(const FHitResult& WallHit);
	void EndWallRun();
	void TickWallRun();
	bool CheckLedge(FHitResult& OutHit);
	void ClimbLedge(const FHitResult& LedgeHit);

	UFUNCTION()
	void OnClimbLedgeFinished();

	void TickMeshTilt(float DeltaTime);

	// 수상보
	void CheckWaterRun();
	void TickWaterRun();

	// 수영
	void CheckInWater();
	void TickSwim();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SMove|State", meta = (AllowPrivateAccess = "true"))
	EMoveState CurrentMoveState;

	ACharacter* MyPlayer;
	AMyPlayer* MyPlayerOwner;

	float WalkSpeed = 300.f;
	float RunSpeed = 600.f;
	float JumpVelocity = 600.f;

	float GlideSpeed = 600.f;
	float GlideGravityScale = 0.1f;
	float GlideMinHeight = 0.0f;
	float GlideDescentSpeed = 100.f; 
	float FastGlideSpeed = 1200.f;
	float FastGlideDescentSpeed = 100.f;
	float LineTraceStartOffset = -100.f;
	float LineTraceLength = 5000.f;

	float WallRunSpeed = 600.0f;
	float GetStaminaUsageRate() const;

	FParkourTraceSettings WallTraceSettings;

	FParkourTraceSettings LedgeTraceSettings;

	FVector WallNormal;

	bool bOriginalOrientRotationToMovement;
	bool bOriginalUseControllerRotationYaw;

	float TargetMeshPitch = 0.0f;
	float TiltInterpSpeed = 3.0f;
	float WallJumpForce = 500.0f;
};