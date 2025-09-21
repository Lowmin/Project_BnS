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
	void OnInventoryChanged(int32 idx, const class UItem* data, const EItemCategory& highlightCategory);
	void OnEquipChanged(int32 idx, const class UItem* data);
	void OnSoulShieldChanged(int32 idx, const class UItem* data);
	void OnJewelSlotChanged(int32 jewelSlotIndex,const class UJewelItem* data);

	// View to Model
public:
	void OnInventoryOpen();
	void OnInventorySort();
	void OnItemUse(int32 index);
	void OnUnEquipToSlot(int32 equipIdx, int32 inventoryIdx);
	void OnSwapItemSlot(int32 indexA, int32 indexB);
	void OnUnEquip(int32 index);
	void OnEquip(int32 inventoryIdx, int32 equipIdx);
	void OnUnEquipSoulShield(int32 index);
	void OnUnEquipSoulShieldToSlot(int32 soulShieldIndex, int32 inventoryIndex);
	void OnEquipJewel(int32 inventotyIdx, int32 jewelSlotIdx);
	void OnUnEquipJewel(int32 jewelSlotIndex);
	void OnUnEquipJewelToSlot(int32 jewelSlotIndex, int32 inventoryIndex);
	void OnEquipSoulShield(int32 inventoryIdx);
	void OnHighlightItem(const EItemCategory highlightCategory);
};
