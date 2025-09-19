// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EquipItem.h"
#include "WeaponItem.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_BNS_API UWeaponItem : public UEquipItem
{
	GENERATED_BODY()

private:
	int32 JewelSlotCount = 6;
	
	UPROPERTY()
	TArray<TObjectPtr<class UJewelItem>> JewelList;

public:
	void SetJewel(int jewelIndex, class UJewelItem* data);
};
