// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "EquipItem.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_BNS_API UEquipItem : public UItem
{
	GENERATED_BODY()

public:
	virtual FString GetItemTypeText() const override;
	virtual FString GetItemOptionText() const override;
	
public:
	EEquipDetailCategory DetailCategory = EEquipDetailCategory::Weapon;
	float MaxHp = 0;
	float Atk = 0;
	float Def = 0;

public:
	void SetData(const FEquipData* data);
};
