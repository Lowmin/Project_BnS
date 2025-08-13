// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SkillBase.h"
#include "MeleeSkill.generated.h"

struct FMeleeData;
class UAnimInstance;

UCLASS()
class PROJECT_BNS_API AMeleeSkill : public ASkillBase
{
	GENERATED_BODY()


public:
	virtual void ExecuteSkill_Implementation() override;

private:
	bool LoadMeleeData();
	void AttackMelee();

	UFUNCTION()
	void OnNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& Payload);

	float AttackRange = 100.f;
	float AttackRadius = 50.f;
	bool  bCanHitMultiTarget = false;
	int32 ComboStepLocal = 1;
	float ChainInputSeconds = 1.f;
	int32 RecoverMPOnThird = 0;
	int32 BossCCOnThird = 0;
	FName NextComboRow = NAME_None;
};
