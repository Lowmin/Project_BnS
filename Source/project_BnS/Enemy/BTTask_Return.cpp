// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Return.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

UBTTask_Return::UBTTask_Return()
{
	NodeName = TEXT("Return Position");
}

EBTNodeResult::Type UBTTask_Return::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AAIController* AIController = OwnerComp.GetAIOwner();

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!AIController || !BlackboardComp) return EBTNodeResult::Failed;

	FVector HomeLocation = BlackboardComp->GetValueAsVector(ReturnPositionKey.SelectedKeyName);

	AIController->MoveToLocation(HomeLocation, 100.0f);

	return EBTNodeResult::Succeeded;
}
