// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_MeleeAttack.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "../Enemy.h"
#include "../Skill/SkillSystemComponent.h"

UBTTask_MeleeAttack::UBTTask_MeleeAttack()
{
	NodeName = TEXT("Melee Attack");
}

EBTNodeResult::Type UBTTask_MeleeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AAIController* AIController = Cast<AAIController>(OwnerComp.GetAIOwner());
	
	if (AIController == nullptr) return EBTNodeResult::Failed;

	AIController->StopMovement();

    AEnemy* Enemy = Cast<AEnemy>(AIController->GetPawn());
    if (!Enemy) return EBTNodeResult::Failed;

    USkillSystemComponent* SkillSystem = Enemy->FindComponentByClass<USkillSystemComponent>();
    if (!SkillSystem) return EBTNodeResult::Failed;

    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    if (!BlackboardComp) return EBTNodeResult::Failed;

    AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject(TargetActorKey.SelectedKeyName));
    if (!TargetActor) return EBTNodeResult::Failed;

    FVector LookDirection = TargetActor->GetActorLocation() - Enemy->GetActorLocation();
    LookDirection.Z = 0;
    FRotator TargetRotation = LookDirection.Rotation();
    Enemy->SetActorRotation(TargetRotation);

    SkillSystem->EnemyUseBasicMelee(AttackSkillID, TargetActor);

    return EBTNodeResult::Succeeded;
}
