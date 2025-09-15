// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Popup.h"
#include "InventoryPopup.generated.h"

DECLARE_DELEGATE(DFele_InventoryOpen);

/**
 * 
 */
UCLASS()
class PROJECT_BNS_API UInventoryPopup : public UPopup
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UItemList> ItemList;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UEquipSlot> WeaponSlot = nullptr;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UGridPanel> EquipRoot = nullptr;
	
	TArray<class UEquipSlot*> EquipSlots;

public:
	virtual void NativeConstruct() override;
	virtual void SetVisiblePopup(bool isVisible) override;

	DFele_InventoryOpen OnInventoryOpen;

	void SetItemSlot(int32 idx, const class UItem* data) const;
	void SetEquipSlot(int32 idx, const class UItem* data) const;

	TArray<class UEquipSlot*> GetEquipList() const;
	class UItemList* GetItemList() const;
};
