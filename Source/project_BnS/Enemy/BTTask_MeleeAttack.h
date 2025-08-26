// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_MeleeAttack.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_BNS_API UBTTask_MeleeAttack : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_MeleeAttack();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere, Category = "Skill")
	int32 AttackSkillID;

	UPROPERTY(EditAnywhere, Category = Blackboard)
	FBlackboardKeySelector TargetActorKey;
	
};
