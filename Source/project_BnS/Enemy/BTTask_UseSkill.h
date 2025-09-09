// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_UseSkill.generated.h"

/**
 * 
 */
class ASkillBase;

struct FUseSkillMemory
{
	TWeakObjectPtr<ASkillBase> SpawnedSkillActor;
};

UCLASS()
class PROJECT_BNS_API UBTTask_UseSkill : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_UseSkill();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override; 
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual uint16 GetInstanceMemorySize() const override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	UPROPERTY(EditAnywhere, Category = "Skill")
	int32 SkillIDToExecute;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector TargetActorKey;
};
