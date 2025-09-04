// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Buff.h"
#include "BuffTick.h"
#include "BuffPoison.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_BNS_API UBuffPoison : public UBuff, public IBuffTick
{
	GENERATED_BODY()
	
public:
	virtual void BuffTick(float deltaTime) override;

private:
	float LastTickTime = 0.0f;
};
