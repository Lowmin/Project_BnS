// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StatItem.h"
#include "EquipItem.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_BNS_API UEquipItem : public UStatItem
{
	GENERATED_BODY()

public:
	virtual FString GetItemTypeText() const override;
	virtual FString GetItemOptionText() const override;
	
public:
	EEquipDetailCategory DetailCategory = EEquipDetailCategory::Weapon;

public:
	void SetData(const FEquipData* data);
};
