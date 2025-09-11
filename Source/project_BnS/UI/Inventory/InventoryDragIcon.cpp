// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryDragIcon.h"

#include "Components/Image.h"

UInventoryDragIcon* UInventoryDragIcon::SetIcon(UTexture2D* texture)
{
	ImgIcon->SetBrushFromTexture(texture);

	return this;
}
