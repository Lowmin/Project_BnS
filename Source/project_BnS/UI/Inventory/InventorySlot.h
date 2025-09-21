// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventorySlotType.h"
#include "Blueprint/UserWidget.h"
#include "InventorySlot.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_BNS_API UInventorySlot : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UInventorySlot(const FObjectInitializer& ObjectInitializer);

protected:
	virtual FReply NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

public:
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

private:
	TSubclassOf<UUserWidget> DragIconClass;
	TWeakObjectPtr<class UInventoryPopup> InventoryPopup = nullptr;
	TObjectPtr<const class UItem> ItemData = nullptr;

protected:
	EInventorySlotType SlotType = EInventorySlotType::ItemSlot;
	
protected:
	virtual void OnMouseRightClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	virtual class UInventoryDragDropOperation* CreateDragOperation(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	virtual void OnDrop(class UInventoryDragDropOperation* dragDropOperation);
	void SetItemData(const class UItem* data);
	void ShowItemInfo();
	void HideItemInfo();

public:
	void SetInventoryPopup(class UInventoryPopup* popup);
	const class UItem* GetItemData() const;
};
