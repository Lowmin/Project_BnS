// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryPopup.h"

#include "EquipSlot.h"
#include "ItemList.h"
#include "Components/GridPanel.h"
#include "Components/Button.h"

void UInventoryPopup::NativeConstruct()
{
	Super::NativeConstruct();

	SelfShortCut = EKeys::I;
	BtnSort->OnClicked.AddDynamic(this, &UInventoryPopup::InventorySort);

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

void UInventoryPopup::SetVisiblePopup(bool isVisible)
{
	Super::SetVisiblePopup(isVisible);

	if (OnInventoryOpen.IsBound())
	{
		OnInventoryOpen.Execute();
	}
	ItemList->HighlightAll();
}

void UInventoryPopup::SetItemSlot(int32 idx, const UItem* data, bool isHighlight) const
{
	ItemList->SetItemSlot(idx, data, isHighlight);
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

void UInventoryPopup::InventorySort()
{
	GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Green, "sort inv");
	if (OnInventorySort.IsBound())
	{
		OnInventorySort.Execute();
	}
}