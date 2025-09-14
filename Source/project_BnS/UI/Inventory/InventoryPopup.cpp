// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryPopup.h"

#include "EquipSlot.h"
#include "ItemList.h"
#include "Components/GridPanel.h"

void UInventoryPopup::NativeConstruct()
{
	Super::NativeConstruct();

	SelfShortCut = EKeys::I;

	// 무기슬롯 등록
	EquipSlots.Add(WeaponSlot);
	WeaponSlot->SetIndex(0);
	// 장비슬롯 등록 
	TArray<UWidget*> childs = EquipRoot->GetAllChildren();
	for(int i=0; i<childs.Num(); ++i)
	{
		UEquipSlot* equip = Cast<UEquipSlot>(childs[i]);
		if(equip == nullptr)
			continue;

		EquipSlots.Add(equip);
		equip->SetIndex(i+1);
	}
}

void UInventoryPopup::SetItemSlot(int32 idx, const UItem* data) const
{
	ItemList->SetItemSlot(idx, data);
}

void UInventoryPopup::SetEquipSlot(int32 idx, const UItem* data) const
{
	if(idx >= EquipSlots.Num())
		return;

	EquipSlots[idx]->SetInfo(data);
}

TArray<UEquipSlot*> UInventoryPopup::GetEquipList() const
{
	return EquipSlots;
}

UItemList* UInventoryPopup::GetItemList() const
{
	return ItemList;
}
