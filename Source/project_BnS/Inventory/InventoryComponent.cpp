// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"
#include "ItemData.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	static ConstructorHelpers::FObjectFinder<UDataTable> itemData(TEXT("/Game/ItemData.ItemData"));
	if (itemData.Succeeded())
	{
		ItemDataTable = itemData.Object;
	}
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	ParsingData();
	SetInventorySlotCount(40);

	AddItem(1, 1);
}


// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UInventoryComponent::ParsingData()
{
	if (!ItemDataTable)
		return;

	// Load the UDataTable
	TArray<FItemData*> arr;
	ItemDataTable->GetAllRows<FItemData>("itemdata", arr);
	auto a = ItemDataTable->FindRow<FItemData>(TEXT("2"), "");

	if (arr.Num() <= 0)
		return;

	for (FItemData* data : arr)
	{
		DataMap.Add(data->Id, data);
	}
}

int32 UInventoryComponent::FindItemSlotIndex(int32 itemId) const
{
	int emptyIdx = -1;
	for(int i=0; i<ItemList.Num(); ++i)
	{
		if (ItemList[i].Id == itemId)
			return i;
		if (emptyIdx < 0 && ItemList[i].Id == 0)
			emptyIdx = i;
	}

	return emptyIdx;
}

const FItemData* UInventoryComponent::GetItemData(int32 itemId) const
{
	if (DataMap.Contains(itemId))
		return DataMap[itemId];

	return nullptr;
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

	const FItemData* newData = GetItemData(id);
	if (newData == nullptr)
		return;
	ItemList[idx] = *newData;
	ItemList[idx].Count = count;
	ItemList[idx].UpdatedItem = true;

	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, "add item");

	if (OnItemSlotChanged.IsBound())
	{
		OnItemSlotChanged.Execute(idx, ItemList[idx]);
	}
}

void UInventoryComponent::RemoveItem(int32 inventoryIdx)
{
}

void UInventoryComponent::UseItem(int32 inventoryIdx)
{
}

void UInventoryComponent::Unequip(int32 equipIdx)
{
}

void UInventoryComponent::UnequipSoulShield(int32 soulShieldSlotIdx)
{
}

