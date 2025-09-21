// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemInfo.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "../../Inventory/Item.h"
#include "../GoldDisplay.h"
#include "Components/HorizontalBox.h"
#include "project_BnS/Inventory/JewelItem.h"
#include "project_BnS/Inventory/WeaponItem.h"

void UItemInfo::ShowInfo(const UItem* data, const FString& diffText)
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

	const UWeaponItem* weapon = Cast<UWeaponItem>(data);
	if(weapon == nullptr)
	{
		JewelRoot->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		JewelRoot->SetVisibility(ESlateVisibility::HitTestInvisible);
		int jewelSlotCount = weapon->GetJewelSlotCount();
		
		JewelSlot_0->SetVisibility(jewelSlotCount > 0 ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
		JewelSlot_1->SetVisibility(jewelSlotCount > 1 ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
		JewelSlot_2->SetVisibility(jewelSlotCount > 2 ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
		JewelSlot_3->SetVisibility(jewelSlotCount > 3 ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
		JewelSlot_4->SetVisibility(jewelSlotCount > 4 ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
		JewelSlot_5->SetVisibility(jewelSlotCount > 5 ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);

		SetJewelTexture(JewelSlot_0, weapon->GetJewelData(0));
		SetJewelTexture(JewelSlot_1, weapon->GetJewelData(1));
		SetJewelTexture(JewelSlot_2, weapon->GetJewelData(2));
		SetJewelTexture(JewelSlot_3, weapon->GetJewelData(3));
		SetJewelTexture(JewelSlot_4, weapon->GetJewelData(4));
		SetJewelTexture(JewelSlot_5, weapon->GetJewelData(5));
	}
	
	GoldDisplay->SetGold(data->Price, false);
	TextCount->SetText(FText::FromString(FString::Printf(TEXT("Count : %d"), data->Count)));

	const bool bIsDiff = !diffText.IsEmpty();
	TextDiff->SetVisibility(bIsDiff ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
	TextDiff->SetText(FText::FromString(diffText));
}

void UItemInfo::HideInfo()
{
	SetVisibility(ESlateVisibility::Hidden);
	//SetVisibility(ESlateVisibility::Collapsed);
}

void UItemInfo::SetJewelTexture(UImage* image, UJewelItem* jewelData)
{
	if(jewelData == nullptr)
	{
		image->SetBrushFromTexture(nullptr);
		return;
	}
	
	image->SetBrushFromTexture(jewelData->JewelTexture);
}
