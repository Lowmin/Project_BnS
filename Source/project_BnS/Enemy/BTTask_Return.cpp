// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_Return.h"
#include "AIController.h"
#include "GameFramework/Pawn.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_Return::UBTTask_Return()
{
	NodeName = TEXT("Return Position");
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_Return::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AAIController* AIController = OwnerComp.GetAIOwner();
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();

	if (!AIController || !BlackboardComp)
	{
		return EBTNodeResult::Failed;
	}

	APawn* ControlledPawn = AIController->GetPawn();
	if (!ControlledPawn)
	{
		return EBTNodeResult::Failed;
	}

	FVector ReturnPosition = BlackboardComp->GetValueAsVector(ReturnPositionKey.SelectedKeyName);

	AIController->MoveToLocation(ReturnPosition, AcceptanceRadius, true);

	return EBTNodeResult::InProgress;
}

void UBTTask_Return::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController || !AIController->GetPathFollowingComponent())
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	UPathFollowingComponent* PathFollowingComp = AIController->GetPathFollowingComponent();
	const EPathFollowingStatus::Type Status = PathFollowingComp->GetStatus();

	if (Status == EPathFollowingStatus::Idle)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
	else if (Status != EPathFollowingStatus::Moving)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}
}