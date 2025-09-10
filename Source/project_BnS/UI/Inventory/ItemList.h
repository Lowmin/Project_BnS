// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/VerticalBox.h"
#include "ItemList.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_BNS_API UItemList : public UUserWidget
{
	GENERATED_BODY()

private:
	TArray<class UItemSlot> ItemSlots;

protected:
	UPROPERTY()
	TSubclassOf<UUserWidget> InventoryRowClass = nullptr;
	UPROPERTY()
	TSubclassOf<UUserWidget> InventoryLockClass = nullptr;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UVerticalBox> Root;

public:
	UItemList(const FObjectInitializer& ObjectInitializer);
	
	virtual bool Initialize() override;

	void SetItemSlot(int idx, const struct FItemData& data);
};
