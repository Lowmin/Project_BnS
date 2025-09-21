// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventorySlot.h"
#include "SoulShieldSlot.generated.h"

DECLARE_DELEGATE_OneParam(FDele_UnEquipSoulShield, int32);
DECLARE_DELEGATE_OneParam(FDele_EquipSoulShield, int32);

/**
 * 
 */
UCLASS()
class PROJECT_BNS_API USoulShieldSlot : public UInventorySlot
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;
	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual void OnMouseRightClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual class UInventoryDragDropOperation* CreateDragOperation(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	virtual void OnDrop(class UInventoryDragDropOperation* dragDropOperation);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> SoulShield0 = nullptr;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> SoulShield1 = nullptr;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> SoulShield2 = nullptr;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> SoulShield3 = nullptr;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> SoulShield4 = nullptr;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> SoulShield5 = nullptr;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> SoulShield6 = nullptr;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> SoulShield7 = nullptr;

	TArray<TObjectPtr<const class UItem>> SoulShieldData;

private:
	int32 GetSoulShieldIndex(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) const;
	void SetSoulShieldTexture(int index, class UImage* soulShieldImage, const class UItem* data);

public:
	FDele_EquipSoulShield OnEquipSoulShield;
	FDele_UnEquipSoulShield OnUnEquipSoulShield;
	void SetInfo(int32 idx, const class UItem* data);
	const class UItem* GetSoulShieldData(const int32 soulShieldIndex) const;
};
