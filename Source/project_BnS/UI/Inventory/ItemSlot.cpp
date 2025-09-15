// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemSlot.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/RetainerBox.h"
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

	static ConstructorHelpers::FObjectFinder<UMaterial> grayscaleMat(TEXT("/Game/UI/umatGrayScale.umatGrayScale"));
	if (grayscaleMat.Succeeded())
	{
		MatGrayscaleBase = grayscaleMat.Object;

	}
}

void UItemSlot::NativeConstruct()
{
	Super::NativeConstruct();

	MatGrayscale = UMaterialInstanceDynamic::Create(MatGrayscaleBase, this);
	if(MatGrayscale != nullptr)
	{
		Retainer->SetEffectMaterial(MatGrayscale);
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
	
	if (!InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		return FReply::Unhandled();
	}
		
	return FReply::Handled().DetectDrag(this->GetCachedWidget().ToSharedRef(), EKeys::LeftMouseButton);
}

void UItemSlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	UInventoryDragDropOperation* dragDropOperation = NewObject<UInventoryDragDropOperation>();
	dragDropOperation->Source = EDragSource::ItemSlot;
	dragDropOperation->Index = Index;

	dragDropOperation->DefaultDragVisual = CreateWidget<UInventoryDragIcon>(this, DragIconClass)->SetIcon(Texture);

	OutOperation = dragDropOperation;
}

bool UItemSlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	UInventoryDragDropOperation* dragDropOperation = Cast<UInventoryDragDropOperation>(InOperation);

	if(dragDropOperation->Source == EDragSource::ItemSlot)
	{
		if (OnSwapItemSlot.IsBound())
		{
			OnSwapItemSlot.Execute(dragDropOperation->Index, Index);
		}
	}
	else
	{
		if(OnUnEquipToSlot.IsBound())
		{
			OnUnEquipToSlot.Execute(dragDropOperation->Index, Index);
		}
	}

	return true;
}

void UItemSlot::SetGrayscale(bool isGray)
{
	MatGrayscale->SetScalarParameterValue(TEXT("Percent"), isGray ? 1.f : 0.f);
}

void UItemSlot::SetIndex(int32 index)
{
	Index = index;
}

void UItemSlot::SetInfo(const UItem* data, bool isHighlight)
{
	if (data == nullptr)
	{
		Texture = nullptr;
		ImgIcon->SetBrushFromTexture(Texture);
		ImgIcon->SetColorAndOpacity(FColor::Red);
		TextCount->SetVisibility(ESlateVisibility::Hidden);
		ImgNewBadge->SetVisibility(ESlateVisibility::Hidden);
		SetGrayscale(false);
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
	SetGrayscale(!isHighlight);
}