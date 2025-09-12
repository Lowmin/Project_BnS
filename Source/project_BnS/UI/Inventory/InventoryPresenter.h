// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "InventoryPresenter.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_BNS_API UInventoryPresenter : public UObject
{
	GENERATED_BODY()
	
private:
	TWeakObjectPtr<class AMyPlayer> Player = nullptr;
	TWeakObjectPtr<class UInventoryPopup> InventoryPopup = nullptr;

public:
	void SetPlayer(class AMyPlayer* player);
	void SetInventoryPopup(class UInventoryPopup* popup);

	// Model to View
public:
	void OnInventoryChanged(int32 idx, const class UItem* data);

	// View to Model
public:
	void OnItemUse(int32 index);
	void OnSwapItemSlot(int32 indexA, int32 indexB);
};
