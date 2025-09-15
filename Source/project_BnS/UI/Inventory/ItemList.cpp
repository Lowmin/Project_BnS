// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemList.h"

#include "EquipSlot.h"
#include "Components/VerticalBox.h"
#include "Components/HorizontalBox.h"
#include "Components/Button.h"
#include "ItemSlot.h"
#include "../../Inventory/ItemData.h"
#include "../../Inventory/Item.h"

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

		UHorizontalBox* box = Cast<UHorizontalBox>(widget->GetRootWidget());
		for (int j = 0; j < box->GetChildrenCount(); ++j)
		{
			UItemSlot* itemSlot = Cast<UItemSlot>(box->GetChildAt(j));
			itemSlot->SetIndex(ItemSlots.Num());
			ItemSlots.Add(itemSlot);
		}
	}

	for (int i = 5; i < 11; ++i)
	{
		UUserWidget* widget = CreateWidget(this, InventoryLockClass);
		Root->AddChild(widget);
	}
	
	return true;
}

void UItemList::NativeConstruct()
{
	Super::NativeConstruct();

	BtnAll->OnClicked.AddDynamic(this, &UItemList::HighlightAll);
	BtnEquip->OnClicked.AddDynamic(this, &UItemList::HighlightEquip);
	BtnSoulShield->OnClicked.AddDynamic(this, &UItemList::HighlightSoulShield);
	BtnUsable->OnClicked.AddDynamic(this, &UItemList::HighlightUsable);
	BtnMaterial->OnClicked.AddDynamic(this, &UItemList::HighlightMaterial);
	BtnEtc->OnClicked.AddDynamic(this, &UItemList::HighlightEtc);
}

void UItemList::HighlightAll()
{
	if (OnHighlightItem.IsBound())
	{
		OnHighlightItem.Execute(EItemCategory::All);
	}
}

void UItemList::HighlightEquip()
{
	if (OnHighlightItem.IsBound())
	{
		OnHighlightItem.Execute(EItemCategory::Equip);
	}
}

void UItemList::HighlightSoulShield()
{
	if (OnHighlightItem.IsBound())
	{
		OnHighlightItem.Execute(EItemCategory::SoulShield);
	}
}

void UItemList::HighlightUsable()
{
	if (OnHighlightItem.IsBound())
	{
		OnHighlightItem.Execute(EItemCategory::Usable);
	}
}

void UItemList::HighlightMaterial()
{
	if (OnHighlightItem.IsBound())
	{
		OnHighlightItem.Execute(EItemCategory::Material);
	}
}

void UItemList::HighlightEtc()
{
	if (OnHighlightItem.IsBound())
	{
		OnHighlightItem.Execute(EItemCategory::Etc);
	}
}

void UItemList::SetItemSlot(int32 idx, const UItem* data, bool isHighlight)
{
	if (idx >= ItemSlots.Num())
		return;

	ItemSlots[idx]->SetInfo(data, isHighlight);
}

TArray<class UItemSlot*> UItemList::GetItemSlotList() const
{
	return ItemSlots;
}
