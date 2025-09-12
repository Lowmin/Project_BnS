// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipItem.h"

void UEquipItem::SetData(const FEquipData* data)
{
	Super::SetData(data);

	DetailCategory = data->DetailCategory;
	Hp = data->Hp;
	Atk = data->Atk;
}
