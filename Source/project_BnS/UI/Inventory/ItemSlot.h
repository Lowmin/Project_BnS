// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemSlot.generated.h"

DECLARE_DELEGATE_TwoParams(FDele_SwapItemSlot, int32, int32);

/**
 * 
 */
UCLASS()
class PROJECT_BNS_API UItemSlot : public UUserWidget
{
	GENERATED_BODY()

public:
	UItemSlot(const FObjectInitializer& ObjectInitializer);

	// Input
protected:
	virtual FReply NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

private:
	int32 Index = 0;
	
protected:
	TObjectPtr<UTexture2D> Texture = nullptr;
	TSubclassOf<UUserWidget> DragIconClass;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UImage> ImgIcon;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UImage> ImgNewBadge;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UTextBlock> TextCount;

public:
	FDele_SwapItemSlot OnSwapItemSlot;

	void SetIndex(int32 index);
	void SetInfo(UTexture2D* texture, bool isNewBadge, int count = 1);
};
