// Fill out your copyright notice in the Description page of Project Settings.

#include "BTService_CalculateReturnPos.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "GameFramework/Pawn.h" 
#include "DrawDebugHelpers.h"

UBTService_CalculateReturnPos::UBTService_CalculateReturnPos()
{
	NodeName = TEXT("Check Distance From Home");
	Interval = 1.0f;
	RandomDeviation = 0.2f;
}

void UBTService_CalculateReturnPos::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AAIController* AIController = OwnerComp.GetAIOwner();
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();

	if (!AIController || !BlackboardComp) return;

	APawn* ControlledPawn = AIController->GetPawn();
	if (!ControlledPawn) return;

	if (BlackboardComp->GetValueAsBool(ShouldReturnKey.SelectedKeyName))
	{
		return;
	}

	FVector HomePosition = BlackboardComp->GetValueAsVector(HomePositionKey.SelectedKeyName);
	float Distance = FVector::Dist(ControlledPawn->GetActorLocation(), HomePosition);

    FString DebugText = FString::Printf(
        TEXT("Dist: %.0f / Max: %.0f\nCompleteDist: %.0f\nReturning: %s"),
        Distance,
        MaxDistance,
        ReturnCompleteDistance,
        BlackboardComp->GetValueAsBool(ShouldReturnKey.SelectedKeyName) ? TEXT("TRUE") : TEXT("FALSE")
    );
    DrawDebugString(GetWorld(), ControlledPawn->GetActorLocation() + FVector(0, 0, 50.0f), DebugText, nullptr, FColor::White, Interval + 0.1f);

	if (Distance > MaxDistance)
	{
		BlackboardComp->SetValueAsBool(ShouldReturnKey.SelectedKeyName, true);
	}
}