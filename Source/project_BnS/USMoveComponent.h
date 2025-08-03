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

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSMoveStateChanged, EMoveState, NewState);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PROJECT_BNS_API USMoveComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USMoveComponent();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(BlueprintAssignable, Category = "SMove")
	FOnSMoveStateChanged OnSMoveStateChanged;

	// 현재 경공 상태
	UPROPERTY(BlueprintReadOnly, Category = "SMove")
	EMoveState CurrentMoveState;

	void StartSMove();
	void StopSMove();

	void StartGlide();
	void StopGlide();

	void StartWallRun();
	void StopWallRun();

	void StartWaterRun();
	void StopWaterRun();

private:
	void SetSMoveState(EMoveState NewState);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SMove|State", meta = (AllowPrivateAccess = "true"))
	bool bIsSMove;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SMove|State", meta = (AllowPrivateAccess = "true"))
	bool bIsGliding;
	UPROPERTY(EditAnywhere, Category = "SMove|Gliding")
	float GlideSpeed;
	UPROPERTY(EditAnywhere, Category = "SMove|Gliding")
	FVector GlideDirection;
	UPROPERTY(EditAnywhere, Category = "SMove|Gliding")
	float GlideStartHeight;
	UPROPERTY(EditAnywhere, Category = "SMove|Gliding")
	float GlideMinHeight;

	UPROPERTY(EditAnywhere, Category = "SMove|WallRun")
	float WallRunDuration;

	UPROPERTY(EditAnywhere, Category = "SMove|WaterRun")
	float WaterRunCheckRadius;

	bool CheckForWall();
	bool CheckForWater();
};