// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryPopup.h"

#include "ItemList.h"

void UInventoryPopup::NativeConstruct()
{
	Super::NativeConstruct();

	SelfShortCut = EKeys::I;
}

void UInventoryPopup::SetItemSlot(int idx, const FItemData& data)
{
}
