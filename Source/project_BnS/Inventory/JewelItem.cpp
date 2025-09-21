// Fill out your copyright notice in the Description page of Project Settings.


#include "JewelItem.h"

void UJewelItem::SetData(const FJewelData* data)
{
	Super::SetData(data);

	JewelTexture = data->JewelTexture;
}
