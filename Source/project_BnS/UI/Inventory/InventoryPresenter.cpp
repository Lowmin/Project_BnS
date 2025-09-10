// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryPresenter.h"

#include "../../MyPlayer.h"
#include "../../Inventory/InventoryComponent.h"
#include "../../Inventory/ItemData.h"
#include "InventoryPopup.h"

void UInventoryPresenter::SetPlayer(AMyPlayer* player)
{
	Player = player;

	Player->GetInventoryComponent()->OnItemSlotChanged.BindUObject(this, &UInventoryPresenter::OnInventoryChanged);
}

void UInventoryPresenter::SetInventoryPopup(UInventoryPopup* popup)
{
	InventoryPopup = popup;
}

void UInventoryPresenter::OnInventoryChanged(int idx, const FItemData& data)
{
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, FString::FromInt(idx) + " : " + FString::FromInt(data.Id));

	if (InventoryPopup == nullptr)
		return;

	InventoryPopup->SetItemSlot(idx, data);
}
