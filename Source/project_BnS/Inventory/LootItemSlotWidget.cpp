// Fill out your copyright notice in the Description page of Project Settings.


#include "LootItemSlotWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void ULootItemSlotWidget::SetItemInfo(const FItemData* ItemData, int32 Quantity)
{
	if (!ItemData) return;

	if (ItemIcon && ItemData->Icon)
	{
		ItemIcon->SetBrushFromTexture(ItemData->Icon);
	}
	if (ItemName)
	{
		ItemName->SetText(FText::FromString(ItemData->ItemName));
	}
	if (ItemQuantity)
	{
		ItemQuantity->SetText(FText::AsNumber(Quantity));
		ItemQuantity->SetVisibility(Quantity > 1 ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	}
}