// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "BuffType.h"
#include "BuffData.generated.h"

USTRUCT(BlueprintType)
struct FBuffData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	int32 Idx = 0;
	UPROPERTY(EditAnywhere)
	EBuffType Type = EBuffType::Poison;
	UPROPERTY(EditAnywhere)
	float Value = 0.0f;
	UPROPERTY(EditAnywhere)
	float TickTime = 1.0f;
	UPROPERTY(EditAnywhere)
	float Duration = 10.0f;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UBuff> BuffClass = nullptr;
	UPROPERTY(EditAnywhere)
	UTexture2D* Icon = nullptr;
};