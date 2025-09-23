// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemData.h"
#include "LootItemSlotWidget.generated.h"

class UImage;
class UTextBlock;

UCLASS()
class PROJECT_BNS_API ULootItemSlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetItemInfo(const FItemData* ItemData, int32 Quantity);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> ItemIcon;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemName;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemQuantity;
};