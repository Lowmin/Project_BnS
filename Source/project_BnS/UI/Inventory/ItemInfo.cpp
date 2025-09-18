// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemInfo.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "../../Inventory/Item.h"
#include "../GoldDisplay.h"

void UItemInfo::ShowInfo(const UItem* data)
{
	if (data == nullptr)
	{
		HideInfo();
		return;
	}

	SetVisibility(ESlateVisibility::HitTestInvisible);

	TextItemName->SetText(FText::FromString(data->GetItemName()));
	switch (data->Rarity)
	{
	case EItemRarity::Common:
		TextItemName->SetColorAndOpacity(FColor::White);
		break;
	case EItemRarity::UnCommon:
		TextItemName->SetColorAndOpacity(FColor(30, 255, 0));
		break;
	case EItemRarity::Rare:
		TextItemName->SetColorAndOpacity(FColor(0, 112, 221));
		break;
	case EItemRarity::Epic:
		TextItemName->SetColorAndOpacity(FColor(163, 53, 238));
		break;
	case EItemRarity::Legendary:
		TextItemName->SetColorAndOpacity(FColor(255, 128, 0));
		break;
	}

	ImgIcon->SetBrushFromTexture(data->Icon);
	TextType->SetText(FText::FromString(data->GetItemTypeText()));
	TextOption->SetText(FText::FromString(data->GetItemOptionText()));
	TextDescript->SetText(FText::FromString(data->Description));
	GoldDisplay->SetGold(data->Price, false);
	TextCount->SetText(FText::FromString(FString::Printf(TEXT("Count : %d"), data->Count)));
	TextDiff->SetVisibility(ESlateVisibility::Collapsed);
}

void UItemInfo::HideInfo()
{
	SetVisibility(ESlateVisibility::Hidden);
	//SetVisibility(ESlateVisibility::Collapsed);
}
