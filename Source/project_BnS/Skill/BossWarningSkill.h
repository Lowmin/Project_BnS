// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MeleeSkill.h"
#include "WarningIndicator.h"
#include "BossWarningSkill.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_BNS_API ABossWarningSkill : public AMeleeSkill
{
	GENERATED_BODY()

public:
	virtual void ExecuteSkill_Implementation() override;
	ABossWarningSkill();

protected:
	void PerformDelayedAttack();

private:
	UPROPERTY(EditDefaultsOnly, Category = "Config")
	TSubclassOf<AWarningIndicator> WarningIndicatorClass;

	UPROPERTY(EditDefaultsOnly, Category = "Config")
	float WarningDuration = 3.0f;

	UPROPERTY()
	TObjectPtr<AWarningIndicator> SpawnedIndicator;
};
