// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "UsableItem.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_BNS_API UUsableItem : public UItem
{
	GENERATED_BODY()

public:
	UPROPERTY()
	int32 BuffID = 0;

public:
	void SetData(const FUsableItemData* data);
};
