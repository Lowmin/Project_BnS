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
	float AttackRange = 200.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float AttackRadius = 50.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bCanHitMultiTarget = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 ComboStep = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly) 
	FName HitNotifyName = TEXT("Hit");



};