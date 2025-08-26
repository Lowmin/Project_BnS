// Fill out your copyright notice in the Description page of Project Settings.

#include "BTService_CalculateDistance.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_CalculateDistance::UBTService_CalculateDistance()
{
	NodeName = TEXT("Calculate Distance To Target");
	Interval = 0.1f;
}

void UBTService_CalculateDistance::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    AAIController* AIController = OwnerComp.GetAIOwner();
    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    if (!AIController || !BlackboardComp) return;

    AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject(TEXT("TargetActor")));
    APawn* ControlledPawn = AIController->GetPawn();

    if (TargetActor && ControlledPawn)
    {
        float Distance = ControlledPawn->GetDistanceTo(TargetActor);
        BlackboardComp->SetValueAsFloat(GetSelectedBlackboardKey(), Distance);
    }
    else
    {
        BlackboardComp->ClearValue(GetSelectedBlackboardKey());
    }
}