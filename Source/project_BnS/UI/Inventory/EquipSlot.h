// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EquipSlot.generated.h"

DECLARE_DELEGATE_OneParam(FDele_UnEquip, int32);
DECLARE_DELEGATE_TwoParams(FDele_Equip, int32, int32);

/**
 * 
 */
UCLASS()
class PROJECT_BNS_API UEquipSlot : public UUserWidget
{
	GENERATED_BODY()

public:
	UEquipSlot(const FObjectInitializer& ObjectInitializer);

	// Input
private:
	TSubclassOf<UUserWidget> DragIconClass;
	UPROPERTY()
	TObjectPtr<UTexture2D> Texture = nullptr;
	
protected:
	virtual FReply NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

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
