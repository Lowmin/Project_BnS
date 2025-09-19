#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
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
	void SetMoveState(EMoveState NewState);
	EMoveState GetMoveState() const;

	void SMoveToggle();
	void GlideToggle();
	void FastGlideToggle();
	void SJump();
	void WallJump();
	void JumpOnWater();

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Trace")
	TEnumAsByte<EPhysicalSurface> WaterSurfaceType;

private:
	void TickRunning();
	void Glide(float DeltaTime);
	void TickWallRun();
	void TickSwim();
	void TickWaterRun();

	void SetMovementSpeed(EMoveState NewState);
	void UpdateWaterStates();

	void StartSMove();
	void StopSMove();
	void StartGlide();
	void StopGlide();
	void BeginWallRun(const FHitResult& WallHit);
	void EndWallRun(bool bShouldJump = true);
	void ClimbLedge(const FHitResult& LedgeHit);
	UFUNCTION() void OnClimbLedgeFinished();

	float CheckGroundDistance();
	void UsedStamina(float DeltaTime);
	bool CanWallRun(FHitResult& OutHit);
	bool CheckWall(FHitResult& OutHit);
	bool CheckLedge(FHitResult& OutHit);
	void TickMeshTilt(float DeltaTime);

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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SMove|Glide", meta = (AllowPrivateAccess = "true"))
	float GroundDistance{};

	FParkourTraceSettings WallTraceSettings;
	FParkourTraceSettings LedgeTraceSettings;

	FVector WallNormal;
	bool bOriginalOrientRotationToMovement;
	bool bOriginalUseControllerRotationYaw;

	float TargetMeshPitch = 0.0f;
	float TiltInterpSpeed = 3.0f;
	float WallJumpForce = 500.f;
};