// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_Return.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_BNS_API UBTTask_Return : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_Return();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector ReturnPositionKey;
};
