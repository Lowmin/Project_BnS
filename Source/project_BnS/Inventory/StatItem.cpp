// Fill out your copyright notice in the Description page of Project Settings.


#include "StatItem.h"

FStatItemDiffData UStatItem::GetDiffData(const UStatItem* diffItem) const
{
	FStatItemDiffData diffData(this);

	if(diffItem == nullptr)
		return diffData;

	diffData -= diffItem;

	return diffData;
}
