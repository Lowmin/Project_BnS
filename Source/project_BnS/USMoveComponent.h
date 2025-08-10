#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "USMoveComponent.generated.h"

USTRUCT(BlueprintType)
struct FParkourTraceSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parkour")
	float MaxReach = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parkour")
	float TraceRadius = 20.0f;
};

UENUM(BlueprintType)
enum class EMoveState : uint8
{
	Idle			UMETA(DisplayName = "Idle"),
	Running			UMETA(DisplayName = "Running"),
	Gliding			UMETA(DisplayName = "Gliding"),
	WallRunning		UMETA(DisplayName = "WallRunning"),
	ClimbingLedge	UMETA(DisplayName = "ClimbingLedge")
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

	// 경공(달리기)
	void StartSMove();
	void StopSMove();
	void SMoveToggle();

	// 글라이드
	void StartGlide();
	void StopGlide();
	void GlideToggle();
	void Glide(float DeltaTime);

	// 점프
	void SJump();

private:
	// 상태 설정
	void SetMovementSpeed(EMoveState NewState);
	float CheckGroundDistance();

	bool CanWallRun(FHitResult& OutHit);
	bool CheckWall(FHitResult& OutHit);
	void BeginWallRun(const FHitResult& WallHit);
	void EndWallRun();
	void TickWallRun();
	bool CheckLedge(FHitResult& OutHit);
	void ClimbLedge(const FHitResult& LedgeHit);

	UFUNCTION()
	void OnClimbLedgeFinished();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SMove|State", meta = (AllowPrivateAccess = "true"))
	EMoveState CurrentMoveState;

	ACharacter* MyPlayer;

	// 기본 이동 설정
	float WalkSpeed = 300.f;
	float RunSpeed = 600.f;
	float JumpVelocity = 600.f;

	// 글라이드 설정
	float GlideSpeed = 700.f;
	float GlideGravityScale = 0.1f;
	float GlideMinHeight = 0.0f;
	float GlideDescentSpeed = 50.f;
	float LineTraceStartOffset = -100.f;
	float LineTraceLength = 5000.f;

	UPROPERTY(EditAnywhere, Category = "Parkour|Wall Run")
	float WallRunSpeed = 500.0f;

	UPROPERTY(EditAnywhere, Category = "Parkour|Wall Run")
	FParkourTraceSettings WallTraceSettings;

	UPROPERTY(EditAnywhere, Category = "Parkour|Ledge")
	FParkourTraceSettings LedgeTraceSettings;

	FVector WallNormal;

	// 캐릭터의 원래 회전 설정 저장용
	bool bOriginalOrientRotationToMovement;
	bool bOriginalUseControllerRotationYaw;
};
