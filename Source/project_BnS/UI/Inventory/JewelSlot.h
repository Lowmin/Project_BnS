// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EquipSlot.h"
#include "JewelSlot.generated.h"

DECLARE_DELEGATE_TwoParams(FDele_Single_UnEquipJewel, int32 jewelSlotIndex, int32 inventoryIndex);

/**
 * 
 */
UCLASS()
class PROJECT_BNS_API UJewelSlot : public UEquipSlot
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;
	virtual class UInventoryDragDropOperation* CreateDragOperation(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	
public:
	virtual void SetInfo(const class UItem* data) override;
};
