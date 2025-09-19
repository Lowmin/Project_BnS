// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "BossEnemy.generated.h"

class ASkillBase;
/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnImmuneChange);

UCLASS()
class PROJECT_BNS_API ABossEnemy : public AEnemy
{
	GENERATED_BODY()
	
public:
	ABossEnemy();
	void SetCCImmune(bool bImmune);
	void SetCurrentSkill(ASkillBase* Skill); 
	UFUNCTION(BlueprintPure, Category = "Boss")
	bool GetCCImmune() const;

	UPROPERTY(BlueprintAssignable, Category = "Boss")
	FOnImmuneChange OnImmuneStateBegan;

	UPROPERTY(BlueprintAssignable, Category = "Boss")
	FOnImmuneChange OnImmuneStateEnded;

protected:
	virtual void CCApplied() override;
	virtual void CCRemoved() override;
	virtual void Tick(float DeltaTime) override;

private:
	bool bIsCCImmune = true;

	UPROPERTY()
	TObjectPtr<ASkillBase> CurrentSkill;
};
