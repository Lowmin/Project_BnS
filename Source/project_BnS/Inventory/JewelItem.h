// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EquipItem.h"
#include "JewelItem.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_BNS_API UJewelItem : public UEquipItem
{
	GENERATED_BODY()

public:
	TObjectPtr<UTexture2D> JewelTexture = nullptr;

	void SetData(const FJewelData* data);
};
