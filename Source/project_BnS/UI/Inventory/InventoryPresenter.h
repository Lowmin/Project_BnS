// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "../../Inventory/ItemData.h"
#include "InventoryPresenter.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_BNS_API UInventoryPresenter : public UObject
{
	GENERATED_BODY()
	
private:
	TWeakObjectPtr<class AMyPlayer> Player = nullptr;
	TWeakObjectPtr<class UInventoryPopup> InventoryPopup = nullptr;

public:
	void SetPlayer(class AMyPlayer* player);
	void SetInventoryPopup(class UInventoryPopup* popup);

	// Model to View
public:
	void OnInventoryChanged(int32 idx, const class UItem* data, const EItemCategory& highlightCategory) const;
	void OnEquipChanged(int32 idx, const class UItem* data) const;

	// View to Model
public:
	void OnInventoryOpen();
	void OnInventorySort();
	void OnItemUse(int32 index);
	void OnUnEquipToSlot(int32 equipIdx, int32 inventoryIdx);
	void OnSwapItemSlot(int32 indexA, int32 indexB);
	void OnUnEquip(int32 index);
	void OnEquip(int32 inventoryIdx, int32 equipIdx);
};
