// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_RotateToPlayer.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_RotateToPlayer::UBTService_RotateToPlayer()
{
	NodeName = TEXT("Rotate To Player");
}

void UBTService_RotateToPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AAIController* AIController = OwnerComp.GetAIOwner();
	UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();

	if (!AIController || !BlackboardComponent) return;

	AActor* TargetActor = Cast<AActor>(BlackboardComponent->GetValueAsObject(TargetKey.SelectedKeyName));

	if (TargetActor)
	{
		AIController->SetFocus(TargetActor);
	}
	else
	{
		AIController->ClearFocus(EAIFocusPriority::Default);
	}
}
