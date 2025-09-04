// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_UseSkill.h"
#include "AIController.h"
#include "../CharacterBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "../Skill/SkillSystemComponent.h"

UBTTask_UseSkill::UBTTask_UseSkill()
{
	NodeName = TEXT("Use Skill By ID");
}

EBTNodeResult::Type UBTTask_UseSkill::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController) return EBTNodeResult::Failed;

	ACharacterBase* OwnerPawn = Cast<ACharacterBase>(AIController->GetPawn());
	if (!OwnerPawn) return EBTNodeResult::Failed;

	USkillSystemComponent* SkillSystem = OwnerPawn->FindComponentByClass<USkillSystemComponent>();

	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	if (!SkillSystem || !Blackboard) return EBTNodeResult::Failed;

	AActor* TargetActor = Cast<AActor>(Blackboard->GetValueAsObject(TargetActorKey.SelectedKeyName));

	if (SkillSystem->EnemyUseBasicMelee(SkillIDToExecute, TargetActor))
	{
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
