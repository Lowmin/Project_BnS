// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_CheckBossHP.h"
#include "AIController.h"
#include "../BossEnemy.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_CheckBossHP::UBTService_CheckBossHP()
{
	NodeName = TEXT("Check Boss HP");
	Interval = 0.5f;
}

void UBTService_CheckBossHP::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AAIController* AIController = OwnerComp.GetAIOwner();
	ABossEnemy* Boss = Cast<ABossEnemy>(AIController->GetPawn());
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();

	if (!Boss || !BlackboardComp) return;

	const float CurrentHealthRatio = Boss->GetStatusComponent()->GetCurHp() / Boss->GetStatusComponent()->GetMaxHp();

	const bool bCanSummon = CurrentHealthRatio < HealthThreshold;
	BlackboardComp->SetValueAsBool(BlackboardKey.SelectedKeyName, bCanSummon);
}
