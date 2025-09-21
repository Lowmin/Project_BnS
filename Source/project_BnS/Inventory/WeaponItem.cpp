// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponItem.h"

#include "JewelItem.h"

void UWeaponItem::SetData(const FWeaponData* data)
{
	Super::SetData(data);

	JewelSlotCount = data->JewelSlotCount;
	JewelList.SetNum(JewelSlotCount);
}

bool UWeaponItem::EquipJewel(const int jewelSlotIndex,UJewelItem* data)
{
	if(jewelSlotIndex < 0)
		return false;
	if (jewelSlotIndex >= JewelList.Num())
		return false;

	if(JewelList[jewelSlotIndex] != nullptr)
		return false;

	JewelList[jewelSlotIndex] = data;
	return true;
}

bool UWeaponItem::IsEmptyJewel(const int jewelSlotIndex)
{
	if(jewelSlotIndex < 0)
		return true;
	if(jewelSlotIndex >= JewelList.Num())
		return true;
	
	return JewelList[jewelSlotIndex] == nullptr; 
}

UJewelItem* UWeaponItem::UnEquipJewel(const int32 jewelSlotIndex)
{
	if(jewelSlotIndex < 0)
		return nullptr;
	if(jewelSlotIndex >= JewelList.Num())
		return nullptr;
	
	TObjectPtr<UJewelItem> temp = JewelList[jewelSlotIndex];
	JewelList[jewelSlotIndex] = nullptr;
	return temp; 
}

UJewelItem* UWeaponItem::GetJewelData(const int32 jewelSlotIndex)
{
	if(jewelSlotIndex < 0)
		return nullptr;
	if(jewelSlotIndex >= JewelList.Num())
		return nullptr;

	return JewelList[jewelSlotIndex];
}

int32 UWeaponItem::GetJewelSlotCount() const
{
	return JewelSlotCount;
}

int32 UWeaponItem::GetEmptyJewelSlotCount() const
{
	int EmptySlot = JewelSlotCount;

	for(const UJewelItem* jewel : JewelList)
	{
		if(jewel != nullptr)
			--EmptySlot;
	}
	
	return EmptySlot;
}
