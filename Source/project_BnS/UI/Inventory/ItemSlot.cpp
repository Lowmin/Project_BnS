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

	if(MatGrayscale == nullptr)
	{
		MatGrayscale = UMaterialInstanceDynamic::Create(MatGrayscaleBase, this);
		if (MatGrayscale != nullptr)
		{
			Retainer->SetEffectMaterial(MatGrayscale);
		}
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

UInventoryDragDropOperation* UItemSlot::CreateDragOperation(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	UInventoryDragDropOperation* dragDropOperation = NewObject<UInventoryDragDropOperation>();
	if (dragDropOperation != nullptr)
	{
		dragDropOperation->Source = EInventorySlotType::ItemSlot;
		dragDropOperation->Index = Index;
	}
	
	return dragDropOperation;
}
void UItemSlot::OnDrop(UInventoryDragDropOperation* dragDropOperation)
{
	Super::OnDrop(dragDropOperation);

	switch (dragDropOperation->Source)
	{
	case EInventorySlotType::ItemSlot:
		if (OnSwapItemSlot.IsBound())
		{
			OnSwapItemSlot.Execute(dragDropOperation->Index, Index);
		}
		break;
	case EInventorySlotType::EquipSlot:
		if (OnUnEquipToSlot.IsBound())
		{
			OnUnEquipToSlot.Execute(dragDropOperation->Index, Index);
		}
		break;
	case EInventorySlotType::SoulShieldSlot:
		if (OnUnEquipSoulShieldToSlot.IsBound())
		{
			OnUnEquipSoulShieldToSlot.Execute(dragDropOperation->Index, Index);
		}
		break;
	case EInventorySlotType::JewelSlot:
		if(OnUnEquipJewelToSlot.IsBound())
		{
			OnUnEquipJewelToSlot.Execute(dragDropOperation->Index, Index);
		}
		break;
	default:
		break;
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
	SetItemData(data);

	if (data == nullptr)
	{
		ImgIcon->SetBrushFromTexture(nullptr);
		ImgIcon->SetColorAndOpacity(FColor::Transparent);
		TextCount->SetVisibility(ESlateVisibility::Hidden);
		ImgNewBadge->SetVisibility(ESlateVisibility::Hidden);
		SetGrayscale(false);
		return;
	}

	ImgIcon->SetBrushFromTexture(data->Icon);
	ImgIcon->SetColorAndOpacity(data->Count > 0 ? FColor::White : FColor::Transparent);

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