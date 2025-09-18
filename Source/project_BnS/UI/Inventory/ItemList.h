// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/VerticalBox.h"
#include "../../Inventory/ItemData.h"
#include "ItemList.generated.h"

DECLARE_DELEGATE_OneParam(FDele_HighlightItem, enum EItemCategory);

/**
 * 
 */
UCLASS()
class PROJECT_BNS_API UItemList : public UUserWidget
{
	GENERATED_BODY()

public:
	UItemList(const FObjectInitializer& ObjectInitializer);
	virtual bool Initialize() override;
	virtual void NativeConstruct() override;

private:
	TArray<class UItemSlot*> ItemSlots;

	TObjectPtr<UTexture2D> TextureAllEnable = nullptr;
	TObjectPtr<UTexture2D> TextureAllDisable = nullptr;
	TObjectPtr<UTexture2D> TextureEquipEnable = nullptr;
	TObjectPtr<UTexture2D> TextureEquipDisable = nullptr;
	TObjectPtr<UTexture2D> TextureSoulShieldEnable = nullptr;
	TObjectPtr<UTexture2D> TextureSoulShieldDisable = nullptr;
	TObjectPtr<UTexture2D> TextureUsableEnable = nullptr;
	TObjectPtr<UTexture2D> TextureUsableDisable = nullptr;
	TObjectPtr<UTexture2D> TextureMaterialEnable = nullptr;
	TObjectPtr<UTexture2D> TextureMaterialDisable = nullptr;
	TObjectPtr<UTexture2D> TextureEtcEnable = nullptr;
	TObjectPtr<UTexture2D> TextureEtcDisable = nullptr;

public:
	UFUNCTION()
	void HighlightAll();
	UFUNCTION()
	void HighlightEquip();
	UFUNCTION()
	void HighlightSoulShield();
	UFUNCTION()
	void HighlightUsable();
	UFUNCTION()
	void HighlightMaterial();
	UFUNCTION()
	void HighlightEtc();

private:
	void SetCategoryImage(enum EItemCategory category);
	void SetButtonImage(UButton* btn, UTexture2D* texture);

protected:
	UPROPERTY()
	TSubclassOf<UUserWidget> InventoryRowClass = nullptr;
	UPROPERTY()
	TSubclassOf<UUserWidget> InventoryLockClass = nullptr;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UVerticalBox> Root;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> BtnAll = nullptr;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> BtnEquip = nullptr;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> BtnSoulShield = nullptr;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> BtnUsable = nullptr;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> BtnMaterial = nullptr;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> BtnEtc = nullptr;


public:
	FDele_HighlightItem OnHighlightItem;
	void SetItemSlot(int32 idx, const class UItem* data, bool isHighlight);

	TArray<class UItemSlot*> GetItemSlotList() const;
	void SetInventoryPopup(class UInventoryPopup* popup);
};
