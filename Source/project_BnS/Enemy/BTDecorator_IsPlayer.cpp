// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_IsPlayer.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTDecorator_IsPlayer::UBTDecorator_IsPlayer()
{
	NodeName = TEXT("Is Player");
}

bool UBTDecorator_IsPlayer::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp) return false;

	AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject(TargetKey.SelectedKeyName));

	if (!TargetActor)
	{
		return false;
	}

	if (TargetActor->ActorHasTag(FName("Enemy")))
	{
		return false;
	}

	return true;
}


