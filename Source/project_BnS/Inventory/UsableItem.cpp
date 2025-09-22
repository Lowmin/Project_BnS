// Fill out your copyright notice in the Description page of Project Settings.


#include "UsableItem.h"

void UUsableItem::SetData(const FUsableItemData* data)
{
	Super::SetData(data);

	BuffID = data->BuffID;
}
