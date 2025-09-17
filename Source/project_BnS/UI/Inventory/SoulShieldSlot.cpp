// Fill out your copyright notice in the Description page of Project Settings.


#include "SoulShieldSlot.h"

#include "Components/Image.h"
#include "../../Inventory/Item.h"

USoulShieldSlot::USoulShieldSlot(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UUserWidget> dragIcon(TEXT("/Game/UI/Inventory/WBP_InventoryDragIcon.WBP_InventoryDragIcon_C"));
	if (dragIcon.Succeeded())
	{
		DragIconClass = dragIcon.Class;
	}
}

void USoulShieldSlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

}

bool USoulShieldSlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	return false;
}

void USoulShieldSlot::OnMouseRightClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::OnMouseRightClick(InGeometry, InMouseEvent);

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

	int index = (degree + 22.5f) / 45.0f;
	if (index > 7)
		index = 0;

	if (OnUnEquipSoulShield.IsBound())
	{
		OnUnEquipSoulShield.Execute(index);
	}

}

void USoulShieldSlot::SetTexture(UImage* image, const class UItem* data)
{
	if (data == nullptr)
	{
		image->SetBrushFromTexture(nullptr);
		image->SetColorAndOpacity(FColor(0, 0, 0, 0));
		return;
	}

	Texture = data->Icon;
	image->SetBrushFromTexture(Texture);
	image->SetColorAndOpacity(FColor::White);
}

void USoulShieldSlot::SetInfo(int32 idx, const UItem* data)
{
	switch (idx)
	{
	case 0:
		SetTexture(SoulShield0, data);
		break;
	case 1:
		SetTexture(SoulShield1, data);
		break;
	case 2:
		SetTexture(SoulShield2, data);
		break;
	case 3:
		SetTexture(SoulShield3, data);
		break;
	case 4:
		SetTexture(SoulShield4, data);
		break;
	case 5:
		SetTexture(SoulShield5, data);
		break;
	case 6:
		SetTexture(SoulShield6, data);
		break;
	case 7:
		SetTexture(SoulShield7, data);
		break;
	default:
		break;
	}
}
