// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"
#include "Item.h"
#include "EquipItem.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	static ConstructorHelpers::FObjectFinder<UDataTable> itemData(TEXT("/Game/DT_EquipData.DT_EquipData"));
	if (itemData.Succeeded())
	{
		EquipDataTable = itemData.Object;
	}
}

// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	ParsingData();
	EquipList.SetNum((int)EEquipDetailCategory::Count);
	SetInventorySlotCount(40);

	AddItem(1, 2);
	AddItem(2, 1);
}


// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UInventoryComponent::ParsingData()
{
	if (!EquipDataTable)
		return;

	// Load the UDataTable
	TArray<FEquipData*> arr;
	EquipDataTable->GetAllRows<FEquipData>("equipData", arr);

	if (arr.Num() <= 0)
		return;

	for (FEquipData* data : arr)
	{
		EquipDataMap.Add(data->Id, data);
	}
}

int32 UInventoryComponent::FindItemSlotIndex(int32 itemId) const
{
	int emptyIdx = -1;
	for(int i=0; i<ItemList.Num(); ++i)
	{
		if (ItemList[i] == nullptr)
		{
			if(emptyIdx < 0)
			{
				emptyIdx = i;
			}

			continue;
		}

		if (ItemList[i]->Id == itemId)
			return i;
	}

	return emptyIdx;
}

UItem* UInventoryComponent::CreateItem(int32 itemId) const
{
	if (!EquipDataMap.Contains(itemId))
		return nullptr;

	UEquipItem* equipItem = NewObject<UEquipItem>(GetOwner());
	equipItem->SetData(EquipDataMap[itemId]);

	return equipItem;

}

void UInventoryComponent::SwapItem(int32 indexA, int32 indexB)
{
	if (indexA == indexB)
		return;
	if (indexA >= ItemList.Num())
		return;
	if (indexB >= ItemList.Num())
		return;

	UItem* temp = ItemList[indexA];
	ItemList[indexA] = ItemList[indexB];
	ItemList[indexB] = temp;

	if(ItemList[indexA] != nullptr)
	{
		ItemList[indexA]->UpdatedItem = false;
	}
	if(ItemList[indexB] != nullptr)
	{
		ItemList[indexB]->UpdatedItem = false;
	}

	if (OnItemSlotChanged.IsBound())
	{
		OnItemSlotChanged.Execute(indexA, ItemList[indexA]);
		OnItemSlotChanged.Execute(indexB, ItemList[indexB]);
	}
	
}

void UInventoryComponent::SetInventorySlotCount(int32 count)
{
	// 인벤토리는 더 작아지지 않음 
	if (ItemList.Num() >= count)
		return;

	ItemList.SetNum(count);
}

void UInventoryComponent::AddItem(int32 id, int32 count)
{
	// 1개 미만 추가 불가 
	if (count <= 0)
		return;

	// 슬롯 부족 시 아이템 추가 불가능 
	int idx = FindItemSlotIndex(id);
	if (idx < 0)
		return;

	UItem* item = CreateItem(id);
	if (item == nullptr)
		return;

	item->Count += count;
	item->UpdatedItem = true;

	ItemList[idx] = item;

	if (OnItemSlotChanged.IsBound())
	{
		OnItemSlotChanged.Execute(idx, ItemList[idx]);
	}
}

void UInventoryComponent::RemoveItem(int32 inventoryIdx)
{
	ItemList[inventoryIdx] = nullptr;

	if (OnItemSlotChanged.IsBound())
	{
		OnItemSlotChanged.Execute(inventoryIdx, ItemList[inventoryIdx]);
	}
}

void UInventoryComponent::UseItem(int32 inventoryIdx)
{
	UItem* item = ItemList[inventoryIdx];
	if (item == nullptr)
		return;

	switch (item->Category)
	{
	case EItemCategory::Equip:
		Equip(inventoryIdx, Cast<UEquipItem>(item));
		break;
	default:
		break;
	}
}

bool UInventoryComponent::IsEquipAbleSlot(int32 equipIdx) const
{
	if (equipIdx >= EquipList.Num())
		return false;

	return EquipList[equipIdx] == nullptr;
}

void UInventoryComponent::Equip(int32 inventoryIdx, int32 equipIdx)
{
	// 장착 할 슬롯 아이템이 없는경우 불가 
	if(inventoryIdx >= ItemList.Num())
		return;
	UItem* item = ItemList[inventoryIdx];
	if(item == nullptr)
		return;

	// 아이템이 장비가 아닌경우 장착 불가 
	UEquipItem* equipItem = Cast<UEquipItem>(item);
	if(equipItem == nullptr)
		return;

	// 아이템 타입이 다른경우 장착 불가 
	if((int)equipItem->DetailCategory != equipIdx)
		return;

	// 장착 슬롯 체크
	Equip(inventoryIdx, equipItem);

	// if(equipIdx >= EquipList.Num())
	// 	return;
	// UEquipItem* equipSlot = EquipList[equipIdx];
	// if(equipSlot != nullptr)
	// {
	// 	EquipList[equipIdx] = nullptr;
	// }
	//
	// // 장비 장착 
	// Equip(inventoryIdx, equipItem);
	//
	// if(equipSlot != nullptr)
	// {
	// 	ItemList[inventoryIdx] = equipSlot;
	// }
}

void UInventoryComponent::Equip(int32 inventoryIdx, UEquipItem* equipItem)
{
	if (equipItem == nullptr)
		return;

	TObjectPtr<UEquipItem> temp = equipItem;
	RemoveItem(inventoryIdx);

	int32 equipIndex = (int32)equipItem->DetailCategory;

	if (EquipList[equipIndex] != nullptr)
	{
		UnEquip(equipIndex, inventoryIdx);
	}

	if (IsEquipAbleSlot(equipIndex))
	{
		EquipList[equipIndex] = temp;

		if(OnEquipSlotChanged.IsBound())
		{
			OnEquipSlotChanged.Execute(equipIndex, temp);
		}
	}

	
	if(OnItemSlotChanged.IsBound())
	{
		OnItemSlotChanged.Execute(inventoryIdx, ItemList[inventoryIdx]);
	}
}

void UInventoryComponent::UnEquip(int32 equipIdx, int32 targetInventoryIdx)
{
	if (targetInventoryIdx < 0)
	{
		targetInventoryIdx = FindItemSlotIndex(-1);
	}

	if (targetInventoryIdx < 0)
		return;

	if(ItemList[targetInventoryIdx] != nullptr)
		return;

	ItemList[targetInventoryIdx] = EquipList[equipIdx];
	EquipList[equipIdx] = nullptr;

	if(OnItemSlotChanged.IsBound())
	{
		OnItemSlotChanged.Execute(targetInventoryIdx, ItemList[targetInventoryIdx]);
	}
	
	if(OnEquipSlotChanged.IsBound())
	{
		OnEquipSlotChanged.Execute(equipIdx, nullptr);
	}
}

void UInventoryComponent::UnEquipSoulShield(int32 soulShieldSlotIdx)
{
}

