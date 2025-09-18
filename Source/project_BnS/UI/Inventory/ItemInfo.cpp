// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemInfo.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "../../Inventory/Item.h"

void UItemInfo::ShowInfo(const UItem* data)
{
	if (data == nullptr)
	{
		HideInfo();
		return;
	}

	SetVisibility(ESlateVisibility::HitTestInvisible);

	TextItemName->SetText(FText::FromString(data->GetItemName()));
	ImgIcon->SetBrushFromTexture(data->Icon);
	TextType->SetText(FText::FromString(data->GetItemTypeText()));
	TextOption->SetText(FText::FromString(data->GetItemOptionText()));
	TextDescript->SetText(FText::FromString(data->Description));
	//GoldDisplay;
	TextCount->SetText(FText::FromString(FString::Printf(TEXT("Count : %d"), data->Count)));
	TextDiff->SetVisibility(ESlateVisibility::Collapsed);
}

void UItemInfo::HideInfo()
{
	SetVisibility(ESlateVisibility::Collapsed);
}
