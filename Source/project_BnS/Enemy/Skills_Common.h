// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "../CrowdControlComponent.h"
#include "NiagaraSystem.h"
#include "Sound/SoundCue.h"
#include "Skills_Common.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct PROJECT_BNS_API FSkills_Common : public FTableRowBase
{
	GENERATED_BODY()

	// 스킬 이름
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText SkillName;

	// 스킬 몽타주
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAnimMontage> Montage;

	// 데미지 배율 (몬스터의 공격력에 곱하여 계산)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DamageMultiplier;

	// 쿨타임
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Cooldown;

	// CC 적용 타입
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ECrowdControlType CCToApply;

	// 스킬 사용 및 적중 파티클 이펙트
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UNiagaraSystem> VFX;

	// 사운드
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USoundCue> SFX;
};
