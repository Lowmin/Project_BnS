// Fill out your copyright notice in the Description page of Project Settings.


#include "SoulShieldSlot.h"

#include "Components/Image.h"
#include "../../Inventory/Item.h"

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
