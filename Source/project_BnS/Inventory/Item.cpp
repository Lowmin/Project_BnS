// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"

void UItem::SetData(const FItemData* data)
{

	Id = data->Id;
	Category = data->Category;
	Rarity = data->Rarity;
	IsStackAble = data->IsStackAble;
	Icon = data->Icon;
	UpdatedItem = data->UpdatedItem;
	Description = data->Description;
}
