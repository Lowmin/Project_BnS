// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BuffType.h"
#include "Buff.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FBuffSetupDele, struct FBuffData);

struct FBuffData
{
	int32 Idx;
	EBuffType Type;
	float Value;
	float TickTime;
	float Duration;
};

/**
 * 
 */
UCLASS()
class PROJECT_BNS_API UBuff : public UObject
{
	GENERATED_BODY()
	
private:
	FBuffData Data;
	float RemainTime;
	class ACharacterBase* Target;

public:
	FBuffData GetBuffData();

	FBuffSetupDele OnBuffStart;
	FBuffSetupDele OnBuffFinish;
};
