// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_CalculateReturnPos.generated.h"

UCLASS()
class PROJECT_BNS_API UBTService_CalculateReturnPos : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTService_CalculateReturnPos();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	FBlackboardKeySelector HomePositionKey;
};