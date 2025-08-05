#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "USMoveComponent.generated.h"

UENUM(BlueprintType)
enum class EMoveState : uint8
{
	Idle        UMETA(DisplayName = "Idle"),
	Running     UMETA(DisplayName = "Running"),
	Jumping     UMETA(DisplayName = "Jumping"),
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
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	EMoveState getMoveState();

public:
	void SetMoveState(EMoveState NewState);
	void StartSMove();
	void StopSMove();
	void SMoveToggle();
	void StartGlide();
	void StopGlide();
	void GlideToggle();
	void SJump();

private:
	EMoveState CurrentMoveState;
	EMoveState PrevMoveState;
	ACharacter* MyPlayer;

	float walkSpeed = 300.f;
	float runSpeed = 600.f;
	float glideSpeed{};
	float glideGravityScale{};
	float glideMinHeight{};
	float checkGroundDistance();
	float lineTraceStartOffset = -100.f;	// 캐릭터 메시 생각해서 수치 조절
	float lineTraceLength = 1000.f;
	float jumpVelocity = 600.f;
	bool bIsSMove;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SMove|State", meta = (AllowPrivateAccess = "true"))
	bool bIsGliding; 

	/*UPROPERTY(EditAnywhere, Category = "SMove|Gliding")
	float GlideDescentSpeed = 200.0f;*/
};