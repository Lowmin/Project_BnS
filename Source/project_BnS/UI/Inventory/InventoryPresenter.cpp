// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryPresenter.h"

#include "EquipSlot.h"
#include "../../MyPlayer.h"
#include "../../Inventory/InventoryComponent.h"
#include "InventoryPopup.h"
#include "ItemList.h"
#include "ItemSlot.h"
#include "../../Inventory/Item.h"

void UInventoryPresenter::SetPlayer(AMyPlayer* player)
{
	Player = player;

	Player->GetInventoryComponent()->OnItemSlotChanged.BindUObject(this, &UInventoryPresenter::OnInventoryChanged);
	Player->GetInventoryComponent()->OnEquipSlotChanged.BindUObject(this, &UInventoryPresenter::OnEquipChanged);
}

void UInventoryPresenter::SetInventoryPopup(UInventoryPopup* popup)
{
	InventoryPopup = popup;

	// 인벤토리 바인드 
	InventoryPopup->OnInventoryOpen.BindUObject(this, &UInventoryPresenter::OnInventoryOpen);
	InventoryPopup->OnInventorySort.BindUObject(this, &UInventoryPresenter::OnInventorySort);

	// 인벤토리 아이템 리스트 바인드 
	UItemList* itemList = InventoryPopup->GetItemList();
	itemList->OnHighlightItem.BindUObject(this, &UInventoryPresenter::OnHighlightItem);


	// 인벤토리 아이템 슬롯 바인드 
	for (UItemSlot* itemSlot : itemList->GetItemSlotList())
	{
		itemSlot->OnItemUse.BindUObject(this, &UInventoryPresenter::OnItemUse);
		itemSlot->OnUnEquipToSlot.BindUObject(this, &UInventoryPresenter::OnUnEquipToSlot);
		itemSlot->OnSwapItemSlot.BindUObject(this, &UInventoryPresenter::OnSwapItemSlot);
	}
	for(UEquipSlot* equipSlot : InventoryPopup->GetEquipList())
	{
		equipSlot->OnUnEquip.BindUObject(this, &UInventoryPresenter::OnUnEquip);
		equipSlot->OnEquip.BindUObject(this, &UInventoryPresenter::OnEquip);
	}
}

void UInventoryPresenter::OnInventoryChanged(int32 idx, const UItem* data, const EItemCategory& highlightCategory) const
{
	if (InventoryPopup == nullptr)
		return;

	bool isHighlight = true;
	if (data != nullptr)
	{
		isHighlight = data->IsHighlight(highlightCategory);
	}

	InventoryPopup->SetItemSlot(idx, data, isHighlight);
}

void UInventoryPresenter::OnEquipChanged(int32 idx, const UItem* data) const
{
	if (InventoryPopup == nullptr)
		return;

	InventoryPopup->SetEquipSlot(idx, data);
}

void UInventoryPresenter::OnInventoryOpen()
{
	Player->GetInventoryComponent()->OnInventoryOpen();
}

void UInventoryPresenter::OnInventorySort()
{
	Player->GetInventoryComponent()->InventorySort();
}

void UInventoryPresenter::OnItemUse(int32 index)
{
	Player->GetInventoryComponent()->UseItem(index);
}

void UInventoryPresenter::OnUnEquipToSlot(int32 equipIdx, int32 inventoryIdx)
{
	Player->GetInventoryComponent()->UnEquip(equipIdx, inventoryIdx);
}

void UInventoryPresenter::OnSwapItemSlot(int32 indexA, int32 indexB)
{
	Player->GetInventoryComponent()->SwapItem(indexA, indexB);
}

void UInventoryPresenter::OnUnEquip(int32 index)
{
	Player->GetInventoryComponent()->UnEquip(index);
}

void UInventoryPresenter::OnEquip(int32 inventoryIdx, int32 equipIdx)
{
	Player->GetInventoryComponent()->Equip(inventoryIdx, equipIdx);
}

void UInventoryPresenter::OnHighlightItem(const EItemCategory highlightCategory)
{
	Player->GetInventoryComponent()->SetHighlightItem(highlightCategory);
}
