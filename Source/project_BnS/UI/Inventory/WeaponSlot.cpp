// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponSlot.h"

#include "JewelSlot.h"
#include "../../Inventory/JewelItem.h"

void UWeaponSlot::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	SlotType = EInventorySlotType::EquipSlot;
}

void UWeaponSlot::AddJewelSlot(UJewelSlot* slot)
{
	EquipJewelList.Add(slot);
}

TArray<TObjectPtr<class UJewelSlot>> UWeaponSlot::GetJewelSlotList() const
{
	return EquipJewelList;
}

void UWeaponSlot::SetInfo(const UItem* data)
{
	Super::SetInfo(data);
}

void UWeaponSlot::SetJewelSlot(int32 jewelSlotIndex, const UJewelItem* data)
{
	if(jewelSlotIndex < 0)
		return;
	if(jewelSlotIndex >= EquipJewelList.Num())
		return;
	
	EquipJewelList[jewelSlotIndex]->SetInfo(data);
}