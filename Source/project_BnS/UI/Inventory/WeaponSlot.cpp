// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponSlot.h"

#include "JewelSlot.h"
#include "../../Inventory/JewelItem.h"
#include "../../Inventory/WeaponItem.h"

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
	
	if (const UWeaponItem* weaponItem = Cast<UWeaponItem>(data))
	{
		int32 jewelSlotCount = weaponItem->GetJewelSlotCount();
		for (int32 i = 0; i < EquipJewelList.Num(); ++i)
		{
			EquipJewelList[i]->SetVisibility(i < jewelSlotCount ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
		}
	}
	else
	{
		for (int32 i = 0; i < EquipJewelList.Num(); ++i)
		{
			EquipJewelList[i]->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void UWeaponSlot::SetJewelSlot(int32 jewelSlotIndex, const UJewelItem* data)
{
	if(jewelSlotIndex < 0)
		return;
	if(jewelSlotIndex >= EquipJewelList.Num())
		return;
	
	EquipJewelList[jewelSlotIndex]->SetInfo(data);
}