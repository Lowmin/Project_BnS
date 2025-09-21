// Fill out your copyright notice in the Description page of Project Settings.


#include "JewelSlot.h"

#include "InventoryDragDropOperation.h"
#include "../../Inventory/JewelItem.h"

void UJewelSlot::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	SlotType = EInventorySlotType::JewelSlot;
}

UInventoryDragDropOperation* UJewelSlot::CreateDragOperation(const FGeometry& InGeometry,
                                                             const FPointerEvent& InMouseEvent)
{
	UInventoryDragDropOperation* dragDropOperation = NewObject<UInventoryDragDropOperation>();
	if(dragDropOperation != nullptr)
	{
		dragDropOperation->Source = EInventorySlotType::JewelSlot;
		dragDropOperation->Index = Index;
	}

	return dragDropOperation;
}

void UJewelSlot::SetInfo(const UItem* data)
{
	const UJewelItem* jewelItem = Cast<UJewelItem>(data);
	
	SetItemData(jewelItem);

	if(jewelItem == nullptr)
	{
		ImgIcon->SetBrushFromTexture(nullptr);
		ImgIcon->SetColorAndOpacity(FColor::Red);
		return;
	}
	
	ImgIcon->SetBrushFromTexture(jewelItem->JewelTexture);
	ImgIcon->SetColorAndOpacity(FColor::White);
}
