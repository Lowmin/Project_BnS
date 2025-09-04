// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_IsReady.h"
#include "AIController.h"
#include "../CharacterBase.h"
#include "../Skill/SkillSystemComponent.h"

UBTDecorator_IsReady::UBTDecorator_IsReady()
{
	NodeName = TEXT("Skill Ready Check");
}

bool UBTDecorator_IsReady::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController) return false;

	ACharacterBase* OwnerPawn = Cast<ACharacterBase>(AIController->GetPawn());
	if (!OwnerPawn) return false;

	USkillSystemComponent* SkillSystem = OwnerPawn->GetSkillSystemComponent();
	if (!SkillSystem) return false;

	return SkillSystem->IsReady(SkillID);
}
