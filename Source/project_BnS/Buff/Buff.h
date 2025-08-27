// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BuffData.h"
#include "Buff.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FBuffSetupDele, FBuffData);


/**
 * 
 */
UCLASS()
class PROJECT_BNS_API UBuff : public UObject
{
	GENERATED_BODY()
	
protected:
	const FBuffData* Data;
	float RemainTime = 0.0f;
	TWeakObjectPtr<class ACharacterBase> Target;

public:
	void SetBuffData(const FBuffData* data);
	void SetTarget(ACharacterBase* target);
	const FBuffData& GetBuffData() const;

	FBuffSetupDele OnBuffStart;
	FBuffSetupDele OnBuffFinish;
};
