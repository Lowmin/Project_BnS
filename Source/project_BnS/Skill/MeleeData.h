// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "MeleeData.generated.h"

USTRUCT(BlueprintType)
struct FMeleeData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float AttackRange = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float AttackRadius = 50.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bCanHitMultiTarget = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 ComboStep = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName NextComboRow;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ChainInput = 1.0f;	// 다음 입력 대기 시간

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 RecoverMPOnThird = 2;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 BossCCOnThird = 1;	// 보스 합격기 카운트

};