// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "../Enemy.h"
#include "Engine/DecalActor.h"
#include "SkillBase_Boss.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EPatternType : uint8
{
	None			UMETA(DisplayName = "없음"),
	Normal			UMETA(DisplayName = "일반 패턴"),
	Warning			UMETA(DisplayName = "예고형 패턴"),
	Summon			UMETA(DisplayName = "소환 패턴")
};

USTRUCT(BlueprintType)
struct PROJECT_BNS_API FSkillBase_Boss : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAnimMontage> Montage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DamageMultiplier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EPatternType PatternType;

	// 예고형 패턴에서 나오는 공격범위 원
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ADecalActor> WarningCircle;

	// 원이 나온 후 공격에 이르기까지 시간
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ExcutionDelay;

	// 일반 몬스터 소환 패턴에서 사용할 몬스터 블루프린트
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AEnemy> SummonEnemy;
	
	// 특정 강제 패턴 시, 연계될 패턴 지정(Default는 None)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FName> NextPattern;

};
