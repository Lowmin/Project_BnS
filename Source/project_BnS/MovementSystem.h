#pragma once

#include "CoreMinimal.h"
#include "project_BnS/CharacterBase.h"
#include "MovementSystem.generated.h"

UCLASS()
class PROJECT_BNS_API AMovementSystem : public ACharacterBase
{
	GENERATED_BODY()

public:
	AMovementSystem();

protected:
	virtual void BeginPlay() override;

public:
	void MoveForward(float Value);
	void MoveRight(float Value);
	void Look(float YawValue, float PitchValue);

	void BeginRunning();
	void StopRunning();

};