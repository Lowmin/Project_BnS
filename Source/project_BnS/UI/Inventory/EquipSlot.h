// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventorySlot.h"
#include "EquipSlot.generated.h"

DECLARE_DELEGATE_OneParam(FDele_UnEquip, int32);
DECLARE_DELEGATE_TwoParams(FDele_Equip, int32, int32);

/**
 * 
 */
UCLASS()
class PROJECT_BNS_API UEquipSlot : public UInventorySlot
{
	GENERATED_BODY()

protected:
	virtual void OnMouseRightClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual class UInventoryDragDropOperation* CreateDragOperation() override;
	virtual void OnDrop(class UInventoryDragDropOperation* dragDropOperation) override;

	// Item
private:
	int32 Index = 0;
	
protected:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UImage> ImgIcon = nullptr;

public:
	FDele_Equip OnEquip;
	FDele_UnEquip OnUnEquip;
	void SetIndex(int32 index);
	void SetInfo(const class UItem* data);
};
