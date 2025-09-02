// Fill out your copyright notice in the Description page of Project Settings.

#include "BTService_CalculateReturnPos.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "GameFramework/Pawn.h"

UBTService_CalculateReturnPos::UBTService_CalculateReturnPos()
{
	NodeName = TEXT("Calculate Distance From ReturnPos");
	bNotifyBecomeRelevant = true;
}

void UBTService_CalculateReturnPos::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	AAIController* AIController = OwnerComp.GetAIOwner();
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!AIController || !BlackboardComp) return;

	APawn* ControlledPawn = AIController->GetPawn();
	if (!ControlledPawn) return;

	const FRotator CurrentRotation = ControlledPawn->GetActorRotation();
	BlackboardComp->SetValueAsRotator(DefaultRotationKey.SelectedKeyName, CurrentRotation);
}

void UBTService_CalculateReturnPos::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AAIController* AIController = OwnerComp.GetAIOwner();
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!AIController || !BlackboardComp) return;

	APawn* ControlledPawn = AIController->GetPawn();
	if (!ControlledPawn || !BlackboardComp->IsVectorValueSet(HomePositionKey.SelectedKeyName))
	{
		BlackboardComp->ClearValue(GetSelectedBlackboardKey());
		return;
	}

	const FVector HomePosition = BlackboardComp->GetValueAsVector(HomePositionKey.SelectedKeyName);
	const float Distance = FVector::Dist(ControlledPawn->GetActorLocation(), HomePosition);

	BlackboardComp->SetValueAsFloat(GetSelectedBlackboardKey(), Distance);
}