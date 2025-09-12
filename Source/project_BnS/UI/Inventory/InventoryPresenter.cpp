// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryPresenter.h"

#include "../../MyPlayer.h"
#include "../../Inventory/InventoryComponent.h"
#include "../../Inventory/ItemData.h"
#include "InventoryPopup.h"
#include "ItemList.h"
#include "ItemSlot.h"

void UInventoryPresenter::SetPlayer(AMyPlayer* player)
{
	Player = player;

	Player->GetInventoryComponent()->OnItemSlotChanged.BindUObject(this, &UInventoryPresenter::OnInventoryChanged);
}

void UInventoryPresenter::SetInventoryPopup(UInventoryPopup* popup)
{
	InventoryPopup = popup;

	// 인벤토리 아이템 슬롯 바인드 
	UItemList* itemList = InventoryPopup->GetItemList();
	for (UItemSlot* itemSlot : itemList->GetItemSlotList())
	{
		itemSlot->OnItemUse.BindUObject(this, &UInventoryPresenter::OnItemUse);
		itemSlot->OnSwapItemSlot.BindUObject(this, &UInventoryPresenter::OnSwapItemSlot);
	}
}

void UInventoryPresenter::OnInventoryChanged(int32 idx, const UItem* data)
{
	if (InventoryPopup == nullptr)
		return;

	InventoryPopup->SetItemSlot(idx, data);
}

void UInventoryPresenter::OnItemUse(int32 index)
{
	Player->GetInventoryComponent()->UseItem(index);
}

void UInventoryPresenter::OnSwapItemSlot(int32 indexA, int32 indexB)
{
	Player->GetInventoryComponent()->SwapItem(indexA, indexB);
}
