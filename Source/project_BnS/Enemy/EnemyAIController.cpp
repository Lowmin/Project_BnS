// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"
#include "../Enemy.h"
#include "../EnemyData.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

AEnemyAIController::AEnemyAIController()
{
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));

	UAISenseConfig_Sight* SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("AISenseConfigSIght"));

	SightConfig->SightRadius = 1500.0f;													// 감지반경
	SightConfig->LoseSightRadius = 2000.0f;												// 타겟과의 거리 반경
	SightConfig->PeripheralVisionAngleDegrees = 90.0f;									// 시야각
	SightConfig->SetMaxAge(5.0f);														// 대상 기억 시간

	// 모두 ture로 해야 플레이어 감지 가능
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;							// 적 감지
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;							// 중립 감지
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;						// 아군 감지

	AIPerceptionComponent->ConfigureSense(*SightConfig);
	AIPerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());
}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyAIController::TargetPercception);

	AEnemy* PossessedEnemy = Cast<AEnemy>(InPawn);

	if (PossessedEnemy)
	{
		const FDataTableRowHandle& EnemyDataHandle = PossessedEnemy->GetDataHandle();

		if (EnemyDataHandle.DataTable && !EnemyDataHandle.RowName.IsNone())
		{
			const FEnemyData* EnemyData = EnemyDataHandle.GetRow<FEnemyData>(TEXT(""));

			if (EnemyData && EnemyData->BehaviorTree)
			{
				BlackboardComponent->InitializeBlackboard(*(EnemyData->BehaviorTree->BlackboardAsset));
				BlackboardComponent->SetValueAsFloat(AttackRangeKey, EnemyData->D_AttackRange);
				BlackboardComponent->SetValueAsVector(ReturnPositionKey, InPawn->GetActorLocation());
				BehaviorTreeComponent->StartTree(*(EnemyData->BehaviorTree));
			}
		}
	}
}

void AEnemyAIController::TargetPercception(AActor* Actor, FAIStimulus Stimulus)
{
	if (!Actor)
	{
		BlackboardComponent->ClearValue(TargetNameKey);
		return;
	}

	if (Actor->ActorHasTag(FName("Enemy")))
	{
		return;
	}

	if (Stimulus.WasSuccessfullySensed())
	{
		BlackboardComponent->SetValueAsObject(TargetNameKey, Actor);
	}
}


