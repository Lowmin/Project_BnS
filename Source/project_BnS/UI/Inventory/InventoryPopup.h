// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Popup.h"
#include "../../Inventory/ItemData.h"
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
	virtual void NativeConstruct() override;
	virtual void SetVisiblePopup(bool isVisible) override;

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UItemList> ItemList = nullptr;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UEquipSlot> WeaponSlot = nullptr;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UGridPanel> EquipRoot = nullptr;
	
	UPROPERTY()
	TArray<class UEquipSlot*> EquipSlots;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class USoulShieldSlot> SoulShieldSlot = nullptr;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> BtnSort = nullptr;

public:
	FDele_InventoryOpen OnInventoryOpen;
	FDele_InventorySort OnInventorySort;

	void SetItemSlot(int32 idx, const class UItem* data, bool isHighlight) const;
	void SetEquipSlot(int32 idx, const class UItem* data) const;
	void SetSoulShieldSlot(int32 idx, const class UItem* data) const;
	TArray<class UEquipSlot*> GetEquipList() const;
	class UItemList* GetItemList() const;
	UFUNCTION()
	void InventorySort();
};
