// Fill out your copyright notice in the Description page of Project Settings.


#include "SoulShieldItem.h"

void USoulShieldItem::SetData(const FSoulShieldData* data)
{
	Super::SetData(data);

	DetailCategory = data->DetailCategory;
	MaxHp = data->MaxHp;
	Atk = data->Atk;
	Def = data->Def;
}
