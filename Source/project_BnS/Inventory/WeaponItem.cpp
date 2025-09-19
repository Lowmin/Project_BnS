// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponItem.h"

void UWeaponItem::SetJewel(int jewelIndex, UJewelItem* data)
{
	if (JewelSlotCount <= jewelIndex)
		return;

	JewelList[jewelIndex] = data;
}
