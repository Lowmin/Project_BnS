// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "BehaviorTree/BehaviorTree.h"
#include "EnemyData.generated.h"

USTRUCT(BlueprintType)
struct PROJECT_BNS_API FEnemyData : public FTableRowBase
{
	GENERATED_BODY()

	// 스탯 관련, 앞에 붙은 D는 데이터베이스 자료임을 나타냄
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString D_EnemyName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float D_MaxHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float D_Atk;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float D_Def;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int D_Level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float D_AttackRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UAnimMontage> HitReactMontage;

	// 몬스터 별 행동을 담는 비헤이비어 트리
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UBehaviorTree> BehaviorTree;
};