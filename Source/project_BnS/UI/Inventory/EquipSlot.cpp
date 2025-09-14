// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipSlot.h"

#include "InventoryDragDropOperation.h"
#include "InventoryDragIcon.h"
#include "Components/Image.h"
#include "project_BnS/Inventory/Item.h"

UEquipSlot::UEquipSlot(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UUserWidget> dragIcon(TEXT("/Game/UI/Inventory/WBP_InventoryDragIcon.WBP_InventoryDragIcon_C"));
	if (dragIcon.Succeeded())
	{
		DragIconClass = dragIcon.Class;
	}
}

FReply UEquipSlot::NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnPreviewMouseButtonDown(InGeometry, InMouseEvent);
	
	if (InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton))
	{
		GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Green, "mouse right");
		if(OnUnEquip.IsBound())
		{
			OnUnEquip.Execute(Index);
		}
		return FReply::Unhandled();
	}

	if (!InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		return FReply::Unhandled();
	}

	return FReply::Handled().DetectDrag(this->GetCachedWidget().ToSharedRef(), EKeys::LeftMouseButton);
}

void UEquipSlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
	UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
	
	UInventoryDragDropOperation* dragDropOperation = NewObject<UInventoryDragDropOperation>();
	dragDropOperation->Source = EDragSource::EquipSlot;
	dragDropOperation->Index = Index;

	dragDropOperation->DefaultDragVisual = CreateWidget<UInventoryDragIcon>(this, DragIconClass)->SetIcon(Texture);

	OutOperation = dragDropOperation;
}

bool UEquipSlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
	
	UInventoryDragDropOperation* dragDropOperation = Cast<UInventoryDragDropOperation>(InOperation);

	if(dragDropOperation->Source == EDragSource::ItemSlot)
	{
		if (OnEquip.IsBound())
		{
			OnEquip.Execute(dragDropOperation->Index, Index);
		}
	}
	else
	{
		
	}

	return true;
}

void UEquipSlot::SetIndex(int32 index)
{
	Index = index;
}

void UEquipSlot::SetInfo(const class UItem* data)
{
	if(data == nullptr)
	{
		ImgIcon->SetBrushFromTexture(nullptr);
		ImgIcon->SetColorAndOpacity(FColor::Red);
		return;
	}
	
	Texture = data->Icon;
	ImgIcon->SetBrushFromTexture(Texture);
	ImgIcon->SetColorAndOpacity(FColor::White);
}

