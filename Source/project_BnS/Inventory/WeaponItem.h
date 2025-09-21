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

public:
	FStatItemDiffData GetDiffData(const UWeaponItem* diffItem) const;
	void SetData(const FWeaponData* data);
	
private:
	int32 JewelSlotCount = 6;
	
	UPROPERTY()
	TArray<TObjectPtr<class UJewelItem>> JewelList;

public:
	bool EquipJewel(const int jewelSlotIndex,class UJewelItem* data);
	bool IsEmptyJewel(const int jewelSlotIndex);
	class UJewelItem* UnEquipJewel(const int32 jewelSlotIndex);
	class UJewelItem* GetJewelData(const int32 jewelSlotIndex) const;
	int32 GetJewelSlotCount() const;
	int32 GetEmptyJewelSlotCount() const;
};
