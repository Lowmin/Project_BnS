// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_FilterTarget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_BNS_API UBTService_FilterTarget : public UBTService
{
	GENERATED_BODY()
	
public:
	UBTService_FilterTarget();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector TargetKey;
};
