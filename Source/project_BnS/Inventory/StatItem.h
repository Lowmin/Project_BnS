// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "StatItem.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_BNS_API UStatItem : public UItem
{
	GENERATED_BODY()
	
public:
	float MaxHp = 0;
	float Atk = 0;
	float Def = 0;
};
