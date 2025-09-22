// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Buff.h"
#include "BuffTick.h"
#include "BuffPotion.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_BNS_API UBuffPotion : public UBuff, public IBuffTick
{
	GENERATED_BODY()

private:
	float LastTickTime = 0.0f;

public:
	virtual void BuffTick(float deltaTime) override;
};
