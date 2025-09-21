// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum EInventorySlotType : uint8
{
	ItemSlot,
	EquipSlot,
	SoulShieldSlot,
	JewelSlot
};