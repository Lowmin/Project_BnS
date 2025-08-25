// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_ChasePlayer.h"
#include "EnemyAIController.h"
#include "../Enemy.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_ChasePlayer::UBTTask_ChasePlayer()
{
	NodeName = TEXT("Target Player");
}

EBTNodeResult::Type UBTTask_ChasePlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AEnemyAIController* AIController = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();

	if (AIController == nullptr || BlackboardComp == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	UObject* TargetObject = BlackboardComp->GetValueAsObject(TargetActorKey.SelectedKeyName);
	AActor* TargetActor = Cast<AActor>(TargetObject);

	if (TargetActor == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	AIController->MoveToActor(TargetActor, 100.0f); 

	return EBTNodeResult::Succeeded;
}
