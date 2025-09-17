// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SkillBase.h"
#include "DashSkill.generated.h"

UCLASS()
class PROJECT_BNS_API ADashSkill : public ASkillBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADashSkill();

	virtual void ExecuteSkill_Implementation() override;
	virtual void OnSkillMontageEnded(bool bInterrupted) override;

protected:
	virtual void Tick(float DeltaTime) override;

private:
	bool bIsDash = false;
	FVector TargetLocation;

	UPROPERTY()
	TEnumAsByte<EMovementMode> PreviousMovementMode;

	UPROPERTY()
	float PreviousMaxWalkSpeed = 0.f;
};
