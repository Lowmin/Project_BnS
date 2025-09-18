// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipSlot.h"

#include "InventoryDragDropOperation.h"
#include "InventoryDragIcon.h"
#include "Components/Image.h"
#include "project_BnS/Inventory/Item.h"

void UEquipSlot::OnMouseRightClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::OnMouseRightClick(InGeometry, InMouseEvent);

	if (OnUnEquip.IsBound())
	{
		OnUnEquip.Execute(Index);
	}
}

UInventoryDragDropOperation* UEquipSlot::CreateDragOperation(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	UInventoryDragDropOperation* dragDropOperation = NewObject<UInventoryDragDropOperation>();
	if(dragDropOperation != nullptr)
	{
		dragDropOperation->Source = EDragSource::EquipSlot;
		dragDropOperation->Index = Index;
	}

	return dragDropOperation;
}

void UEquipSlot::OnDrop(UInventoryDragDropOperation* dragDropOperation)
{
	if (dragDropOperation->Source == EDragSource::ItemSlot)
	{
		if (OnEquip.IsBound())
		{
			OnEquip.Execute(dragDropOperation->Index, Index);
		}
	}
}

void UEquipSlot::SetIndex(int32 index)
{
	Index = index;
}

void UEquipSlot::SetInfo(const class UItem* data)
{
	SetItemData(data);

	if(data == nullptr)
	{
		ImgIcon->SetBrushFromTexture(nullptr);
		ImgIcon->SetColorAndOpacity(FColor::Red);
		return;
	}
	
	ImgIcon->SetBrushFromTexture(data->Icon);
	ImgIcon->SetColorAndOpacity(FColor::White);
}

