// USMoveComponent.h
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "USMoveComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PROJECT_BNS_API USMoveComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USMoveComponent();

protected:
	virtual void BeginPlay() override;

	void StartWallClimb();
	void StopWallClimb();
	bool CanWallClimb() const;

private:
	bool bIsWallClimbing;
	UPROPERTY(EditAnywhere, Category = "WallClimb")
	float WallClimbSpeed;
	UPROPERTY(EditAnywhere, Category = "WallClimb")
	float WallCheckDistance;
};