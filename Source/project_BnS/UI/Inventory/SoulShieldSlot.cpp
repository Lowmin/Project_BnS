// Fill out your copyright notice in the Description page of Project Settings.


#include "SoulShieldSlot.h"

#include "Components/Image.h"
#include "InventoryDragDropOperation.h"
#include "../../Inventory/Item.h"
#include "../../Inventory/SoulShieldItem.h"

void USoulShieldSlot::NativeConstruct()
{
	Super::NativeConstruct();

	SoulShieldData.SetNum(8);
}
FReply USoulShieldSlot::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	int32 index = GetSoulShieldIndex(InGeometry, InMouseEvent);
	SetItemData(SoulShieldData[index]);
	ShowItemInfo();

	return Super::NativeOnMouseMove(InGeometry, InMouseEvent);
}


void USoulShieldSlot::OnMouseRightClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::OnMouseRightClick(InGeometry, InMouseEvent);


	if (OnUnEquipSoulShield.IsBound())
	{
		int32 index = GetSoulShieldIndex(InGeometry, InMouseEvent);
		OnUnEquipSoulShield.Execute(index);
	}
}

class UInventoryDragDropOperation* USoulShieldSlot::CreateDragOperation(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::CreateDragOperation(InGeometry, InMouseEvent);


	UInventoryDragDropOperation* dragDropOperation = NewObject<UInventoryDragDropOperation>();
	if (dragDropOperation != nullptr)
	{
		int32 index = GetSoulShieldIndex(InGeometry, InMouseEvent);

		dragDropOperation->Source = EDragSource::SoulShieldSlot;
		dragDropOperation->Index = index;
		SetItemData(SoulShieldData[index]);
	}

	return dragDropOperation;
}
void USoulShieldSlot::OnDrop(class UInventoryDragDropOperation* dragDropOperation)
{ 
	Super::OnDrop(dragDropOperation);

	if (dragDropOperation->Source == EDragSource::ItemSlot)
	{
		// equip ss
		if (OnEquipSoulShield.IsBound())
		{
			OnEquipSoulShield.Execute(dragDropOperation->Index);
		}
	}
}

int32 USoulShieldSlot::GetSoulShieldIndex(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) const
{
	FVector2D v2 = InGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition());
	FVector2D center = FVector2D(50.f, 50.f);

	FVector2D dir = v2 - center;
	dir.Normalize();
	dir.Y *= -1;

	float degree = FMath::RadiansToDegrees(FMath::Acos(FVector2D::DotProduct(dir, FVector2D(0.0f, 1.0f))));
	if (FVector2D::DotProduct(dir, FVector2D(1.0f, 0.0f)) < 0)
	{
		degree = 360.0f - degree;
	}

	int32 index = (degree + 22.5f) / 45.0f;
	if (index > 7)
		index = 0;
	
	return index;
}

void USoulShieldSlot::SetSoulShieldTexture(int index, UImage* soulShieldImage, const class UItem* data)
{
	const USoulShieldItem* soulShield = Cast<USoulShieldItem>(data);

	SoulShieldData[index] = data;

	if (soulShield == nullptr)
	{
		soulShieldImage->SetBrushFromTexture(nullptr);
		soulShieldImage->SetColorAndOpacity(FColor(0, 0, 0, 0));
		return;
	}

	soulShieldImage->SetBrushFromTexture(soulShield->SoulShieldTexture);
	soulShieldImage->SetColorAndOpacity(FColor::White);
}

void USoulShieldSlot::SetInfo(int32 idx, const UItem* data)
{
	switch (idx)
	{
	case 0:
		SetSoulShieldTexture(idx, SoulShield0, data);
		break;
	case 1:
		SetSoulShieldTexture(idx, SoulShield1, data);
		break;
	case 2:
		SetSoulShieldTexture(idx, SoulShield2, data);
		break;
	case 3:
		SetSoulShieldTexture(idx, SoulShield3, data);
		break;
	case 4:
		SetSoulShieldTexture(idx, SoulShield4, data);
		break;
	case 5:
		SetSoulShieldTexture(idx, SoulShield5, data);
		break;
	case 6:
		SetSoulShieldTexture(idx, SoulShield6, data);
		break;
	case 7:
		SetSoulShieldTexture(idx, SoulShield7, data);
		break;
	default:
		break;
	}
}
