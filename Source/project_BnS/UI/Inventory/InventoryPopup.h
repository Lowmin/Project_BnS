// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventorySlotType.h"
#include "../Popup.h"
#include "project_BnS/Inventory/itemData.h"
#include "project_BnS/Inventory/StatItem.h"
#include "InventoryPopup.generated.h"

DECLARE_DELEGATE(FDele_InventoryOpen);
DECLARE_DELEGATE(FDele_InventorySort);

/**
 * 
 */
UCLASS()
class PROJECT_BNS_API UInventoryPopup : public UPopup
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;
	virtual void SetVisiblePopup(bool isVisible) override;

protected:
	// Weapon Slot
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UWeaponSlot> WeaponSlot = nullptr;

	// Jewel Slot
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UJewelSlot> EquipJewel_0 = nullptr;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UJewelSlot> EquipJewel_1 = nullptr;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UJewelSlot> EquipJewel_2 = nullptr;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UJewelSlot> EquipJewel_3 = nullptr;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UJewelSlot> EquipJewel_4 = nullptr;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UJewelSlot> EquipJewel_5 = nullptr;

	// Equip Slot
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UGridPanel> EquipRoot = nullptr;
	UPROPERTY()
	TArray<class UEquipSlot*> EquipSlots;

	// Soul Shield Slot
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class USoulShieldSlot> SoulShieldSlot = nullptr;

	// Inventory
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UItemInfo> ItemInfo = nullptr;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UItemList> ItemList = nullptr;

	// Bottom Btn
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> BtnSort = nullptr;

public:
	FDele_InventoryOpen OnInventoryOpen;
	FDele_InventorySort OnInventorySort;

	void SetItemSlot(int32 idx, const class UItem* data, bool isHighlight);
	void SetEquipSlot(int32 idx, const class UItem* data);
	void SetSoulShieldSlot(int32 idx, const class UItem* data);
	void SetJewelSlot(int32 jewelSlotIndex, const class UJewelItem* data);
	TArray<class UEquipSlot*> GetEquipList() const;
	class UItemList* GetItemList() const;
	class UWeaponSlot* GetWeaponSlot() const;
	class USoulShieldSlot* GetSoulShieldSlot() const;
	UFUNCTION()
	void InventorySort();
	void ShowItemInfo(EInventorySlotType fromSlot, const class UItem* data);
	const class UStatItem* GetEquipItemData(EEquipDetailCategory category);
};
