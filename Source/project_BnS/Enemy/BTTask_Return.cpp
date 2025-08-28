// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Return.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_Return::UBTTask_Return()
{
	NodeName = TEXT("Return Position");
}

EBTNodeResult::Type UBTTask_Return::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AAIController* AIController = OwnerComp.GetAIOwner();
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();

	if (!AIController)
	{
		return EBTNodeResult::Failed;
	}

	return EBTNodeResult::Type();
}
