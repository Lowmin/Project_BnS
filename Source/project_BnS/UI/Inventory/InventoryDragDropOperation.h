// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "InventorySlotType.h"
#include "InventoryDragDropOperation.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_BNS_API UInventoryDragDropOperation : public UDragDropOperation
{
	GENERATED_BODY()
	
public:
	EInventorySlotType Source = EInventorySlotType::ItemSlot;
	int32 Index;
};
