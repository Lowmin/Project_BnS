// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponItem.h"

#include "JewelItem.h"

FStatItemDiffData UWeaponItem::GetDiffData(const UWeaponItem* diffItem) const
{
	FStatItemDiffData diffData = Super::GetDiffData(diffItem);

	if(diffItem == nullptr)
		return diffData;

	int32  curJewelCnt = GetJewelSlotCount();
	int32 diffJewelCnt = diffItem->GetJewelSlotCount();
	int32  jewelCnt = FMath::Max(curJewelCnt, diffJewelCnt);

	for(int i=0; i<jewelCnt; ++i)
	{
		UJewelItem* curJewel = GetJewelData(i);
		if(curJewel)
		{
			diffData += curJewel;
		}
		
		UJewelItem* diffJewel = diffItem->GetJewelData(i);
		if(diffJewel)
		{
			diffData -= diffJewel;
		}
	}

	return diffData;
}

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

UJewelItem* UWeaponItem::GetJewelData(const int32 jewelSlotIndex) const
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
