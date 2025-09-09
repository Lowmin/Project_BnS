// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_UseSkill.h"
#include "AIController.h"
#include "../CharacterBase.h"
#include "../Skill/SkillSystemComponent.h"
#include "../Skill/SkillBase.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_UseSkill::UBTTask_UseSkill()
{
	NodeName = TEXT("Use Skill By ID");
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_UseSkill::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	FUseSkillMemory* MyMemory = reinterpret_cast<FUseSkillMemory*>(NodeMemory);
	MyMemory->SpawnedSkillActor = nullptr;

	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController) return EBTNodeResult::Failed;

	ACharacterBase* OwnerPawn = Cast<ACharacterBase>(AIController->GetPawn());
	if (!OwnerPawn) return EBTNodeResult::Failed;

	USkillSystemComponent* SkillSystem = OwnerPawn->FindComponentByClass<USkillSystemComponent>();

	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	if (!SkillSystem || !Blackboard) return EBTNodeResult::Failed;

	AActor* TargetActor = Cast<AActor>(Blackboard->GetValueAsObject(TargetActorKey.SelectedKeyName));
	ASkillBase* SpawnedSkill = SkillSystem->EnemyUseBasicMelee(SkillIDToExecute, TargetActor);

	if (SpawnedSkill)
	{
		MyMemory->SpawnedSkillActor = SpawnedSkill;
		return EBTNodeResult::InProgress;
	}

	if (SkillSystem->EnemyUseBasicMelee(SkillIDToExecute, TargetActor))
	{
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}

void UBTTask_UseSkill::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	FUseSkillMemory* MyMemory = reinterpret_cast<FUseSkillMemory*>(NodeMemory);

	if (!MyMemory->SpawnedSkillActor.IsValid())
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}

uint16 UBTTask_UseSkill::GetInstanceMemorySize() const
{
	return sizeof(FUseSkillMemory);
}

EBTNodeResult::Type UBTTask_UseSkill::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	FUseSkillMemory* MyMemory = reinterpret_cast<FUseSkillMemory*>(NodeMemory);
	if (MyMemory->SpawnedSkillActor.IsValid())
	{
		MyMemory->SpawnedSkillActor->Destroy();
	}
	return EBTNodeResult::Aborted;
}
