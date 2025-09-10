// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemList.h"

#include "Components/VerticalBox.h"

UItemList::UItemList(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UUserWidget> inventoryRow(TEXT("/Game/UI/Inventory/WBP_InventoryRow.WBP_InventoryRow_C"));
	if (inventoryRow.Succeeded())
	{
		InventoryRowClass = inventoryRow.Class;
	}
	static ConstructorHelpers::FClassFinder<UUserWidget> inventoryLock(TEXT("/Game/UI/Inventory/WBP_InventoryLock.WBP_InventoryLock_C"));
	if (inventoryLock.Succeeded())
	{
		InventoryLockClass = inventoryLock.Class;
	}
}

bool UItemList::Initialize()
{
	if(!Super::Initialize())
		return false;

	if (InventoryRowClass == nullptr)
		return false;
	if (InventoryLockClass == nullptr)
		return false;

	// Root->ClearChildren();

	for(int i=0; i<5; ++i)
	{
		UUserWidget* widget = CreateWidget(this, InventoryRowClass);
		Root->AddChild(widget);
	}

	for (int i = 5; i < 11; ++i)
	{
		UUserWidget* widget = CreateWidget(this, InventoryLockClass);
		Root->AddChild(widget);
	}
	
	return true;
}

void UItemList::SetItemSlot(int idx, const FItemData& data)
{

}
