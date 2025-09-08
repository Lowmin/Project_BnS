// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_IsPlayer.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_BNS_API UBTDecorator_IsPlayer : public UBTDecorator
{
	GENERATED_BODY()
	
public:
	UBTDecorator_IsPlayer();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

private:
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector TargetKey;
};
