// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Popup.h"
#include "InventoryPopup.generated.h"

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

public:
	virtual void NativeConstruct() override;
	void SetItemSlot(int idx, const struct FItemData& data);
};
