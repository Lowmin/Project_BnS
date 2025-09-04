// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_IsReady.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_BNS_API UBTDecorator_IsReady : public UBTDecorator
{
	GENERATED_BODY()
	
public:
	UBTDecorator_IsReady();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

private:
	UPROPERTY(EditAnywhere, Category = "Skill")
	int32 SkillID{};
};
