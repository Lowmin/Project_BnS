// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SoulShieldSlot.generated.h"

DECLARE_DELEGATE_OneParam(FDele_UnEquipSoulShield, int32);
DECLARE_DELEGATE_TwoParams(FDele_EquipSoulShield, int32, int32);

/**
 * 
 */
UCLASS()
class PROJECT_BNS_API USoulShieldSlot : public UUserWidget
{
	GENERATED_BODY()

public:
	USoulShieldSlot(const FObjectInitializer& ObjectInitializer);

	// Input
private:
	TSubclassOf<UUserWidget> DragIconClass;
	UPROPERTY()
	TObjectPtr<UTexture2D> Texture = nullptr;

protected:
	virtual FReply NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

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

private:
	void SetTexture(class UImage* image, const class UItem* data);

public:
	FDele_EquipSoulShield OnEquipSoulShield;
	FDele_UnEquipSoulShield OnUnEquipSoulShield;
	void SetInfo(int32 idx, const class UItem* data);
};
