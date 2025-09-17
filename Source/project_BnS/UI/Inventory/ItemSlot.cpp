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
	if (MatGrayscale != nullptr)
	{
		Retainer->SetEffectMaterial(MatGrayscale);
	}
}

void UItemSlot::OnMouseRightClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::OnMouseRightClick(InGeometry, InMouseEvent);

	if (OnItemUse.IsBound())
	{
		OnItemUse.Execute(Index);
	}
}

UInventoryDragDropOperation* UItemSlot::CreateDragOperation()
{
	UInventoryDragDropOperation* dragDropOperation = NewObject<UInventoryDragDropOperation>();
	if (dragDropOperation != nullptr)
	{
		dragDropOperation->Source = EDragSource::ItemSlot;
		dragDropOperation->Index = Index;
	}
	
	return dragDropOperation;
}
void UItemSlot::OnDrop(UInventoryDragDropOperation* dragDropOperation)
{
	Super::OnDrop(dragDropOperation);

	if (dragDropOperation->Source == EDragSource::ItemSlot)
	{
		if (OnSwapItemSlot.IsBound())
		{
			OnSwapItemSlot.Execute(dragDropOperation->Index, Index);
		}
	}
	else
	{
		if (OnUnEquipToSlot.IsBound())
		{
			OnUnEquipToSlot.Execute(dragDropOperation->Index, Index);
		}
	}
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
		SetTexture(nullptr);
		ImgIcon->SetBrushFromTexture(nullptr);
		ImgIcon->SetColorAndOpacity(FColor::Red);
		TextCount->SetVisibility(ESlateVisibility::Hidden);
		ImgNewBadge->SetVisibility(ESlateVisibility::Hidden);
		SetGrayscale(false);
		return;
	}

	SetTexture(data->Icon);

	ImgIcon->SetBrushFromTexture(data->Icon);
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