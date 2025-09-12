// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_CheckBossHP.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_BNS_API UBTService_CheckBossHP : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTService_CheckBossHP();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	float HealthThreshold = 0.5f;

};
