// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SkillBase.h"
#include "MeleeSkill.generated.h"

struct FMeleeData;

UCLASS()
class PROJECT_BNS_API AMeleeSkill : public ASkillBase
{
	GENERATED_BODY()

public:
	virtual void ExecuteSkill_Implementation() override;
	virtual void CancelSkill_Implementation() override;

protected:
	virtual void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);

private:
	bool CacheMeleeData();

	void PerformMeleeAttack();

	UFUNCTION()
	void OnNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& Payload);

	void CleanUp();

	const FMeleeData* MeleeData = nullptr;
};
