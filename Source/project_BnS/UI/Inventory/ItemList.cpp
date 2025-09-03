// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemList.h"

#include "Components/VerticalBox.h"

UItemList::UItemList(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UUserWidget> inventoryRow(TEXT("/Game/UI/Inventory/WBP_InventoryRow.WBP_InventoryRow_C"));
	if(inventoryRow.Succeeded())
	{
		InventoryRowClass = inventoryRow.Class;
	}
}

bool UItemList::Initialize()
{
	if(!Super::Initialize())
		return false;

	if(InventoryRowClass == nullptr)
		return false;

	for(int i=0; i<5; ++i)
	{
		UUserWidget* widget = CreateWidget(this, InventoryRowClass);
	}
	
	return true;
}
