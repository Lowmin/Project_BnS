// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "BossEnemy.generated.h"

class ASkillBase;
/**
 * 
 */
UCLASS()
class PROJECT_BNS_API ABossEnemy : public AEnemy
{
	GENERATED_BODY()
	
public:
	void SetCCImmune(bool bImmune);
	void SetCurrentSkill(ASkillBase* Skill); 
	UFUNCTION(BlueprintPure, Category = "Boss")
	bool IsCCImmune() const;

protected:
	virtual void CCApplied() override;
	virtual void CCRemoved() override;
	virtual void Tick(float DeltaTime) override;

private:
	bool bIsCCImmune = true;

	UPROPERTY()
	TObjectPtr<ASkillBase> CurrentSkill;
};
