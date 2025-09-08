// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_FilterTarget.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_FilterTarget::UBTService_FilterTarget()
{
	NodeName = TEXT("Filter Target");
	// 자주 실행 될 필요 없음
	Interval = 0.2f;
}

void UBTService_FilterTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp) return;

	AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject(TargetKey.SelectedKeyName));

	if (TargetActor && TargetActor->ActorHasTag("Enemy"))
	{
		BlackboardComp->ClearValue(TargetKey.SelectedKeyName);
	}
}
