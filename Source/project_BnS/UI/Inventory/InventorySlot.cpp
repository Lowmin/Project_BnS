// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySlot.h"
#include "InventoryDragDropOperation.h"
#include "InventoryDragIcon.h"

UInventorySlot::UInventorySlot(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UUserWidget> dragIcon(TEXT("/Game/UI/Inventory/WBP_InventoryDragIcon.WBP_InventoryDragIcon_C"));
	if (dragIcon.Succeeded())
	{
		DragIconClass = dragIcon.Class;
	}
}

FReply UInventorySlot::NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{

	if (InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton))
	{
		OnMouseRightClick(InGeometry, InMouseEvent);

		return FReply::Unhandled();
	}

	if (!InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		return FReply::Unhandled();
	}

	return FReply::Handled().DetectDrag(this->GetCachedWidget().ToSharedRef(), EKeys::LeftMouseButton);
}

void UInventorySlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	UInventoryDragDropOperation* dragDropOperation = CreateDragOperation(InGeometry, InMouseEvent);
	if (dragDropOperation == nullptr)
		return;
	
	if (IconTexture == nullptr)
		return;
	
	dragDropOperation->DefaultDragVisual = CreateWidget<UInventoryDragIcon>(this, DragIconClass)->SetIcon(IconTexture);
	dragDropOperation->Pivot = EDragPivot::CenterCenter;

	OutOperation = dragDropOperation;
}

bool UInventorySlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	UInventoryDragDropOperation* dragDropOperation = Cast<UInventoryDragDropOperation>(InOperation);
	if (dragDropOperation != nullptr)
	{
		OnDrop(dragDropOperation);
	}

	return true;
}

void UInventorySlot::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
}

void UInventorySlot::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
}

void UInventorySlot::OnMouseRightClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{

}

UInventoryDragDropOperation* UInventorySlot::CreateDragOperation(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return nullptr;
}

void UInventorySlot::OnDrop(UInventoryDragDropOperation* dragDropOperation)
{
}

void UInventorySlot::SetIconTexture(UTexture2D* texture)
{
	IconTexture = texture;
}
