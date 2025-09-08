// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Popup.h"
#include "Inventory.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_BNS_API UInventory : public UPopup
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UItemList> ItemList;
};
