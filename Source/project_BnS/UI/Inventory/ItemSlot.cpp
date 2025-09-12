// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemSlot.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "InventoryDragDropOperation.h"
#include "InventoryDragIcon.h"
#include "../../Inventory/Item.h"

UItemSlot::UItemSlot(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UUserWidget> dragIcon(TEXT("/Game/UI/Inventory/WBP_InventoryDragIcon.WBP_InventoryDragIcon_C"));
	if (dragIcon.Succeeded())
	{
		DragIconClass = dragIcon.Class;
	}
}

FReply UItemSlot::NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnPreviewMouseButtonDown(InGeometry, InMouseEvent);

	if (InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton))
	{
		if (OnItemUse.IsBound())
		{
			OnItemUse.Execute(Index);
		}
		return FReply::Unhandled();
	}
	else if (!InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		return FReply::Unhandled();
	}
		
	return FReply::Handled().DetectDrag(this->GetCachedWidget().ToSharedRef(), EKeys::LeftMouseButton);
}

void UItemSlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	UInventoryDragDropOperation* dragDropOperation = NewObject<UInventoryDragDropOperation>();
	dragDropOperation->Index = Index;

	dragDropOperation->DefaultDragVisual = CreateWidget<UInventoryDragIcon>(this, DragIconClass)->SetIcon(Texture);

	OutOperation = dragDropOperation;
}

bool UItemSlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	UInventoryDragDropOperation* dragDropOperation = Cast<UInventoryDragDropOperation>(InOperation);

	if (OnSwapItemSlot.IsBound())
	{
		OnSwapItemSlot.Execute(dragDropOperation->Index, Index);
	}

	return true;
}

void UItemSlot::SetIndex(int32 index)
{
	Index = index;
}

void UItemSlot::SetInfo(const UItem* data)
{
	if (data == nullptr)
	{
		Texture = nullptr;
		ImgIcon->SetBrushFromTexture(Texture);
		ImgIcon->SetColorAndOpacity(FColor::Red);
		return;
	}

	Texture = data->Icon;

	ImgIcon->SetBrushFromTexture(Texture);
	ImgIcon->SetColorAndOpacity(data->Count> 0 ? FColor::White : FColor(0, 0, 0, 255));

	if(data->IsStackAble)
	{
		TextCount->SetText(FText::AsNumber(data->Count));
		TextCount->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		TextCount->SetVisibility(ESlateVisibility::Hidden);
	}

	ImgNewBadge->SetVisibility(data->UpdatedItem ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}
