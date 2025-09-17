// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventorySlot.h"
#include "ItemSlot.generated.h"

DECLARE_DELEGATE_OneParam(FDele_ItemUse, int32);
DECLARE_DELEGATE_TwoParams(FDele_UnEquipToSlot, int32, int32);
DECLARE_DELEGATE_TwoParams(FDele_SwapItemSlot, int32, int32);

/**
 * 
 */
UCLASS()
class PROJECT_BNS_API UItemSlot : public UInventorySlot
{
	GENERATED_BODY()

public:
	UItemSlot(const FObjectInitializer& ObjectInitializer);
	
protected:
	virtual void NativeConstruct() override;

protected:
	virtual void OnMouseRightClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual class UInventoryDragDropOperation* CreateDragOperation() override;
	virtual void OnDrop(class UInventoryDragDropOperation* dragDropOperation);

private:
	int32 Index = 0;

	void SetGrayscale(bool isGray);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class URetainerBox> Retainer = nullptr;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UMaterial> MatGrayscaleBase = nullptr;

	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> MatGrayscale = nullptr;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UImage> ImgIcon = nullptr;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UImage> ImgNewBadge = nullptr;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UTextBlock> TextCount = nullptr;

public:
	FDele_ItemUse OnItemUse;
	FDele_UnEquipToSlot OnUnEquipToSlot;
	FDele_SwapItemSlot OnSwapItemSlot;

	void SetIndex(int32 index);
	void SetInfo(const class UItem* data, bool isHighlight);
};
