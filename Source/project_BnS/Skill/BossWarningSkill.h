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
	virtual void OnSkillNotify_Hit() override;

protected:
	void PerformDelayedAttack();

private:
	UPROPERTY(EditDefaultsOnly, Category = "Effect", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AActor> WarningIndicatorClass;

	UPROPERTY(EditDefaultsOnly, Category = "Effect", meta = (AllowPrivateAccess = "true"))
	float WarningDuration = 1.5f;

	UPROPERTY()
	TObjectPtr<AActor> SpawnedIndicator;
};
