// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryPopup.h"

#include "EquipSlot.h"
#include "SoulShieldSlot.h"
#include "ItemList.h"
#include "ItemInfo.h"
#include "JewelSlot.h"
#include "WeaponSlot.h"
#include "Components/GridPanel.h"
#include "Components/Button.h"

void UInventoryPopup::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	BtnSort->OnClicked.AddDynamic(this, &UInventoryPopup::InventorySort);

	// 무기슬롯 등록
	EquipSlots.Add(WeaponSlot);
	WeaponSlot->SetIndex(0);
	WeaponSlot->SetInventoryPopup(this);

	// 보석슬롯 등록
	EquipJewel_0->SetIndex(0);
	EquipJewel_0->SetInventoryPopup(this);
	EquipJewel_1->SetIndex(1);
	EquipJewel_1->SetInventoryPopup(this);
	EquipJewel_2->SetIndex(2);
	EquipJewel_2->SetInventoryPopup(this);
	EquipJewel_3->SetIndex(3);
	EquipJewel_3->SetInventoryPopup(this);
	EquipJewel_4->SetIndex(4);
	EquipJewel_4->SetInventoryPopup(this);
	EquipJewel_5->SetIndex(5);
	EquipJewel_5->SetInventoryPopup(this);
	WeaponSlot->AddJewelSlot(EquipJewel_0);
	WeaponSlot->AddJewelSlot(EquipJewel_1);
	WeaponSlot->AddJewelSlot(EquipJewel_2);
	WeaponSlot->AddJewelSlot(EquipJewel_3);
	WeaponSlot->AddJewelSlot(EquipJewel_4);
	WeaponSlot->AddJewelSlot(EquipJewel_5);

	// 장비슬롯 등록 
	TArray<UWidget*> childs = EquipRoot->GetAllChildren();
	for(int i=0; i<childs.Num(); ++i)
	{
		UEquipSlot* equip = Cast<UEquipSlot>(childs[i]);
		if(equip == nullptr)
			continue;

		EquipSlots.Add(equip);
		equip->SetIndex(i+1);
		equip->SetInventoryPopup(this);
	}
	SoulShieldSlot->SetInventoryPopup(this);
	ItemList->SetInventoryPopup(this);
}

void UInventoryPopup::SetVisiblePopup(bool isVisible)
{

	Super::SetVisiblePopup(isVisible);

	// 신규 아이템 갱신 덮어쓰기 방지를 위해 OnInventoryOpen 델리게이트보다 빨리 호출해야함 
	if (isVisible)
	{
		ItemList->HighlightAll();
	}

	if (OnInventoryOpen.IsBound())
	{
		OnInventoryOpen.Execute();
	}

	ItemInfo->HideInfo();

}

void UInventoryPopup::SetItemSlot(int32 idx, const UItem* data, bool isHighlight)
{
	ItemList->SetItemSlot(idx, data, isHighlight);
}

void UInventoryPopup::SetEquipSlot(int32 idx, const UItem* data)
{
	if(idx >= EquipSlots.Num())
		return;

	EquipSlots[idx]->SetInfo(data);
}

void UInventoryPopup::SetSoulShieldSlot(int32 idx, const UItem* data)
{
	SoulShieldSlot->SetInfo(idx, data);
}

void UInventoryPopup::SetJewelSlot(int32 jewelSlotIndex, const UJewelItem* data)
{
	WeaponSlot->SetJewelSlot(jewelSlotIndex, data);
}

TArray<UEquipSlot*> UInventoryPopup::GetEquipList() const
{
	return EquipSlots;
}

UItemList* UInventoryPopup::GetItemList() const
{
	return ItemList;
}

UWeaponSlot* UInventoryPopup::GetWeaponSlot() const
{
	return WeaponSlot;
}

USoulShieldSlot* UInventoryPopup::GetSoulShieldSlot() const
{
	return SoulShieldSlot;
}

void UInventoryPopup::InventorySort()
{
	GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Green, "sort inv");
	if (OnInventorySort.IsBound())
	{
		OnInventorySort.Execute();
	}
}

void UInventoryPopup::ShowItemInfo(const UItem* data)
{
	ItemInfo->ShowInfo(data);
}
