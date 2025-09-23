// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemLootData.h"
#include "LootPopupWidget.generated.h"

class UScrollBox;
class UButton;
class ULootItemSlotWidget;
class ALootBox;

UCLASS()
class PROJECT_BNS_API ULootPopupWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void PopulateItems(const TArray<FDropItemInfo>& Items, ALootBox* OwnerBox);

protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnLootAllButtonClicked();

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UScrollBox> ItemScrollBox;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_LootAll;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<ULootItemSlotWidget> LootItemSlotClass;

private:
	UPROPERTY()
	TArray<FDropItemInfo> LootableItems;

	UPROPERTY()
	TWeakObjectPtr<ALootBox> OwnerLootBox;
};
