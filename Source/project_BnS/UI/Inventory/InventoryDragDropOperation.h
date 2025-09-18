// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "InventoryDragDropOperation.generated.h"

UENUM(BlueprintType)
enum EDragSource : uint8
{
	ItemSlot,
	EquipSlot,
	SoulShieldSlot
};

/**
 * 
 */
UCLASS()
class PROJECT_BNS_API UInventoryDragDropOperation : public UDragDropOperation
{
	GENERATED_BODY()
	
public:
	EDragSource Source = EDragSource::ItemSlot;
	int32 Index;
};
