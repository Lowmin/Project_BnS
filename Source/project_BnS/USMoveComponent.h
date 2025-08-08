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

    // 벽타기
    void StartWallRun(const FHitResult& WallHit);
    void StopWallRun();
    void WallRun(); 

    // 점프
    void SJump();

private:
    void SetMovementSpeed(EMoveState NewState);
    float CheckGroundDistance();
    bool CheckWall(FHitResult& OutHit);

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SMove|State", meta = (AllowPrivateAccess = "true"))
    EMoveState CurrentMoveState;
    EMoveState PrevMoveState;

    ACharacter* MyPlayer;

    float WalkSpeed = 300.f;
    float RunSpeed = 600.f;
    float JumpVelocity = 600.f;

    float GlideSpeed = 700.f;
    float GlideGravityScale = 0.1f;
    float GlideMinHeight = 0.0f;
    float GlideDescentSpeed = 25.f;

    float LineTraceStartOffset = -100.f;
    float LineTraceLength = 5000.f;

    float WallRunSpeed = 600.f;
    float WallCheckDistance = 100.f;
    float WallRunZVelocity = 500.f;
};