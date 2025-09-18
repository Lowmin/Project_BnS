// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"

void UItem::SetData(const FItemData* data)
{

	Id = data->Id;
	ItemName = data->ItemName;
	Category = data->Category;
	Rarity = data->Rarity;
	IsStackAble = data->IsStackAble;
	Icon = data->Icon;
	Description = data->Description;
	Price = data->Price;
}

bool UItem::IsHighlight(EItemCategory category) const
{
	if (category == EItemCategory::All)
		return true;

	return Category == category;
}
FString UItem::GetItemName() const
{
	return ItemName;
}

FString UItem::GetItemTypeText() const
{
	switch (Category)
	{
	case EItemCategory::Equip:
		return "Equip";
	case EItemCategory::SoulShield:
		return "SoulShield";
	case EItemCategory::Usable:
		return "Usable";
	case EItemCategory::Material:
		return "Material";
	case EItemCategory::Etc:
		return "Etc";
	}

	return FString();
}

FString UItem::GetItemOptionText() const
{
	return FString();
}
