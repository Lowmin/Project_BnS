// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"
#include "Item.h"
#include "EquipItem.h"
#include "JewelItem.h"
#include "WeaponItem.h"
#include "SoulShieldItem.h"
#include "../CharacterBase.h"
#include "../StatComponent.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	static ConstructorHelpers::FObjectFinder<UDataTable> weaponData(TEXT("/Game/DT_WeaponData.DT_WeaponData"));
	if (weaponData.Succeeded())
	{
		WeaponDataTable = weaponData.Object;
	}
	
	static ConstructorHelpers::FObjectFinder<UDataTable> jewelData(TEXT("/Game/DT_JewelData.DT_JewelData"));
	if (jewelData.Succeeded())
	{
		JewelDataTable = jewelData.Object;
	}
	
	static ConstructorHelpers::FObjectFinder<UDataTable> equipData(TEXT("/Game/DT_EquipData.DT_EquipData"));
	if (equipData.Succeeded())
	{
		EquipDataTable = equipData.Object;
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> soulShieldData(TEXT("/Game/DT_SoulShieldData.DT_SoulShieldData"));
	if (soulShieldData.Succeeded())
	{
		SoulShieldDataTable = soulShieldData.Object;
	}
}

// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	ACharacterBase* characterBase = Cast<ACharacterBase>(GetOwner());
	if(characterBase != nullptr)
	{
		StatComponent = characterBase->GetStatusComponent();
	}

	// ...
	ParsingWeaponData();
	ParsingJewelData();
	ParsingEquipData();
	ParsingSoulShieldData();

	EquipList.SetNum((int)EEquipDetailCategory::EquipSlotCount);
	SoulShieldList.SetNum(8);
	SetInventorySlotCount(40);

	// Weapon
	AddItem(1);
	AddItem(2);

	// Ring
	AddItem(10000001);
	AddItem(10000002);

	// Earring
	AddItem(11000001);
	AddItem(11000002);

	// Neckless
	AddItem(12000001);
	AddItem(12000002);

	// Bracelit
	AddItem(13000001);
	AddItem(13000002);

	// Jewel
	AddItem(19000001);
	AddItem(19000002);
	AddItem(19000003);
	AddItem(19000004);
	AddItem(19000005);
	AddItem(19000006);

	AddItem(100000001);
	AddItem(100000002);
	AddItem(100000003);
	AddItem(100000004);
	AddItem(100000005);
	AddItem(100000006);
	AddItem(100000007);
	AddItem(100000008);
}


// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UInventoryComponent::ParsingWeaponData()
{
	if (!WeaponDataTable)
		return;

	// Load the UDataTable
	TArray<FWeaponData*> arr;
	WeaponDataTable->GetAllRows<FWeaponData>("weaponData", arr);

	if (arr.Num() <= 0)
		return;

	for (FWeaponData* data : arr)
	{
		WeaponDataMap.Add(data->Id, data);
	}
}

void UInventoryComponent::ParsingJewelData()
{
	if (!EquipDataTable)
		return;

	// Load the UDataTable
	TArray<FJewelData*> arr;
	JewelDataTable->GetAllRows<FJewelData>("jewelData", arr);

	if (arr.Num() <= 0)
		return;

	for (FJewelData* data : arr)
	{
		JewelDataMap.Add(data->Id, data);
	}
}

void UInventoryComponent::ParsingEquipData()
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

void UInventoryComponent::ParsingSoulShieldData()
{
	if (!SoulShieldDataTable)
		return;

	TArray<FSoulShieldData*> arr;
	SoulShieldDataTable->GetAllRows<FSoulShieldData>("soulShieldData", arr);

	for (FSoulShieldData* data : arr)
	{
		SoulShieldDataMap.Add(data->Id, data);
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
	if (itemId < WEAPON_ID_LIMIT)
	{
		if (!WeaponDataMap.Contains(itemId))
			return nullptr;

		UWeaponItem* weaponItem = NewObject<UWeaponItem>(GetOwner());
		weaponItem->SetData(WeaponDataMap[itemId]);

		return weaponItem;
	}
	else if(itemId < EQUIP_ID_LIMIT)
	{
		if(itemId > JEWEL_ID_START)
		{
			if (!JewelDataMap.Contains(itemId))
				return nullptr;
			
			UJewelItem* jewelItem = NewObject<UJewelItem>(GetOwner());
			jewelItem->SetData(JewelDataMap[itemId]);

			return jewelItem;
		}
		
		if (!EquipDataMap.Contains(itemId))
			return nullptr;

		UEquipItem* equipItem = NewObject<UEquipItem>(GetOwner());
		equipItem->SetData(EquipDataMap[itemId]);

		return equipItem;
	}

	if (!SoulShieldDataMap.Contains(itemId))
		return nullptr;

	USoulShieldItem* soulShieldItem = NewObject<USoulShieldItem>(GetOwner());
	soulShieldItem->SetData(SoulShieldDataMap[itemId]);

	return soulShieldItem;

}

void UInventoryComponent::InventorySort()
{
	ItemList.HeapSort([](const TObjectPtr<UItem>& A, const TObjectPtr<UItem>& B) {
		if (A == nullptr)
			return false;
		if (B == nullptr)
			return true;

		return A->Id < B->Id;
		});

	if (OnItemSlotChanged.IsBound())
	{
		for (int i = 0; i < ItemList.Num(); ++i)
		{
			OnItemSlotChanged.Execute(i, ItemList[i], HighlightCategory);
		}
	}
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
		OnItemSlotChanged.Execute(indexA, ItemList[indexA], HighlightCategory);
		OnItemSlotChanged.Execute(indexB, ItemList[indexB], HighlightCategory);
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
		OnItemSlotChanged.Execute(idx, ItemList[idx], HighlightCategory);
	}
}

void UInventoryComponent::RemoveItem(int32 inventoryIdx)
{
	ItemList[inventoryIdx] = nullptr;

	if (OnItemSlotChanged.IsBound())
	{
		OnItemSlotChanged.Execute(inventoryIdx, ItemList[inventoryIdx], HighlightCategory);
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
	case EItemCategory::SoulShield:
		EquipSoulShield(inventoryIdx, Cast<USoulShieldItem>(item));
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
}

void UInventoryComponent::Equip(int32 inventoryIdx, UEquipItem* equipItem)
{
	if (equipItem == nullptr)
		return;
	if(inventoryIdx >= ItemList.Num())
		return;

	TObjectPtr<UEquipItem> temp = equipItem;
	
	// 보석 장착 
	if(equipItem->DetailCategory == EEquipDetailCategory::Jewel)
	{
		EquipJewel(inventoryIdx, -1, Cast<UJewelItem>(equipItem));
		return;
	}
	
	int32 equipIndex = (int32)equipItem->DetailCategory;
	
	if(equipIndex >= EquipList.Num())
		return;

	RemoveItem(inventoryIdx);
	
	if (EquipList[equipIndex] != nullptr)
	{
		UnEquip(equipIndex, inventoryIdx);
	}

	if (IsEquipAbleSlot(equipIndex))
	{
		EquipList[equipIndex] = temp;

		if (StatComponent != nullptr)
		{
			StatComponent->AddExtraMaxHp(EquipList[equipIndex]->MaxHp);
			StatComponent->AddExtraAtk(EquipList[equipIndex]->Atk);
			StatComponent->AddExtraDef(EquipList[equipIndex]->Def);
		}

		if(OnEquipSlotChanged.IsBound())
		{
			OnEquipSlotChanged.Execute(equipIndex, temp);
		}

		// 무기 장착 시 보석 정보 갱신 
		if(equipIndex == (int32)EEquipDetailCategory::Weapon)
		{
			UWeaponItem* weapon = Cast<UWeaponItem>(equipItem);
			if(weapon == nullptr)
			{
				for(int i=0; i<6; ++i)
				{
					if(OnJewelSlotChange.IsBound())
					{
						OnJewelSlotChange.Execute(i, nullptr);
					}
				}
			}
			else
			{
				for(int i=0; i<weapon->GetJewelSlotCount(); ++i)
				{
					OnJewelSlotChange.Execute(i, weapon->GetJewelData(i));	
				}
			}
		}
	}

	
	if(OnItemSlotChanged.IsBound())
	{
		OnItemSlotChanged.Execute(inventoryIdx, ItemList[inventoryIdx], HighlightCategory);
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
	if (equipIdx < 0)
		return;

	if (equipIdx >= EquipList.Num())
		return;
	if (targetInventoryIdx >= ItemList.Num())
		return;

	if(ItemList[targetInventoryIdx] != nullptr)
		return;
	if (EquipList[equipIdx] == nullptr) 
		return;

	if(StatComponent != nullptr)
	{
		StatComponent->AddExtraMaxHp(-(EquipList[equipIdx]->MaxHp));
		StatComponent->AddExtraAtk(-(EquipList[equipIdx]->Atk));
		StatComponent->AddExtraDef(-(EquipList[equipIdx]->Def));
	}

	ItemList[targetInventoryIdx] = EquipList[equipIdx];
	EquipList[equipIdx] = nullptr;

	if(OnItemSlotChanged.IsBound())
	{
		OnItemSlotChanged.Execute(targetInventoryIdx, ItemList[targetInventoryIdx], HighlightCategory);
	}
	
	if(OnEquipSlotChanged.IsBound())
	{
		OnEquipSlotChanged.Execute(equipIdx, nullptr);
	}

	// 무기 장착 해제 시 보석 정보 갱신 
	if(equipIdx == (int32)EEquipDetailCategory::Weapon)
	{
		for(int i=0; i<6; ++i)
		{
			if(OnJewelSlotChange.IsBound())
			{
				OnJewelSlotChange.Execute(i, nullptr);
			}
		}
	}
}

bool UInventoryComponent::IsEquipAbleSoulShieldSlot(int32 equipIdx) const
{
	if (equipIdx < 0)
		return false;

	if (equipIdx >= SoulShieldList.Num())
		return false;

	return SoulShieldList[equipIdx] == nullptr;
}

void UInventoryComponent::EquipSoulShield(int32 inventoryIdx)
{
	// 장착 할 슬롯 아이템이 없는경우 불가 
	if (inventoryIdx >= ItemList.Num())
		return;
	UItem* item = ItemList[inventoryIdx];
	if (item == nullptr)
		return;

	// 아이템이 보패가 아닌경우 장착 불가 
	USoulShieldItem* soulShieldItem = Cast<USoulShieldItem>(item);
	if (soulShieldItem == nullptr)
		return;

	// 장착
	EquipSoulShield(inventoryIdx, soulShieldItem);

}

void UInventoryComponent::EquipSoulShield(int32 inventoryIdx, USoulShieldItem* soulShieldItem)
{
	if (soulShieldItem == nullptr)
		return;

	TObjectPtr<USoulShieldItem> temp = soulShieldItem;
	RemoveItem(inventoryIdx);

	int32 soulShieldIndex = (int32)soulShieldItem->DetailCategory;
	if (SoulShieldList[soulShieldIndex] != nullptr)
	{
		UnEquipSoulShield(soulShieldIndex, inventoryIdx);
	}

	if (IsEquipAbleSoulShieldSlot(soulShieldIndex))
	{
		SoulShieldList[soulShieldIndex] = temp;

		if (StatComponent != nullptr)
		{
			StatComponent->AddExtraMaxHp(SoulShieldList[soulShieldIndex]->MaxHp);
			StatComponent->AddExtraAtk(SoulShieldList[soulShieldIndex]->Atk);
			StatComponent->AddExtraDef(SoulShieldList[soulShieldIndex]->Def);
		}

		if (OnSoulShieldSlotChanged.IsBound())
		{
			OnSoulShieldSlotChanged.Execute(soulShieldIndex, temp);
		}
	}

	if (OnItemSlotChanged.IsBound())
	{
		OnItemSlotChanged.Execute(inventoryIdx, ItemList[inventoryIdx], HighlightCategory);
	}
}

void UInventoryComponent::UnEquipSoulShield(int32 soulShieldSlotIdx, int32 targetInventoryIdx)
{
	if (targetInventoryIdx < 0)
	{
		targetInventoryIdx = FindItemSlotIndex(-1);
	}

	if (targetInventoryIdx < 0)
		return;
	if (soulShieldSlotIdx < 0)
		return;

	if (soulShieldSlotIdx >= SoulShieldList.Num())
		return;
	if (targetInventoryIdx >= ItemList.Num())
		return;

	if (ItemList[targetInventoryIdx] != nullptr)
		return;
	if (SoulShieldList[soulShieldSlotIdx] == nullptr)
		return;

	if (StatComponent != nullptr)
	{
		StatComponent->AddExtraMaxHp(-(SoulShieldList[soulShieldSlotIdx]->MaxHp));
		StatComponent->AddExtraAtk(-(SoulShieldList[soulShieldSlotIdx]->Atk));
		StatComponent->AddExtraDef(-(SoulShieldList[soulShieldSlotIdx]->Def));
	}

	ItemList[targetInventoryIdx] = SoulShieldList[soulShieldSlotIdx];
	SoulShieldList[soulShieldSlotIdx] = nullptr;

	if (OnItemSlotChanged.IsBound())
	{
		OnItemSlotChanged.Execute(targetInventoryIdx, ItemList[targetInventoryIdx], HighlightCategory);
	}

	if (OnSoulShieldSlotChanged.IsBound())
	{
		OnSoulShieldSlotChanged.Execute(soulShieldSlotIdx, nullptr);
	}
}

void UInventoryComponent::EquipJewel(int32 inventoryIdx, int32 jewelSlotIdx)
{
	// 장착 할 슬롯 아이템이 없는경우 불가 
	if(inventoryIdx >= ItemList.Num())
		return;
	UItem* item = ItemList[inventoryIdx];
	if(item == nullptr)
		return;
	
	// 아이템이 장비가 아닌경우 장착 불가 
	UJewelItem* jewelItem = Cast<UJewelItem>(item);
	if(jewelItem == nullptr)
		return;

	EquipJewel(inventoryIdx, jewelSlotIdx, jewelItem);
}

void UInventoryComponent::EquipJewel(int32 inventoryIdx, int32 jewelSlotIdx, UJewelItem* JewelItem)
{
	if(JewelItem == nullptr)
		return;
	if(inventoryIdx >= ItemList.Num())
		return;

	// 무기 장착하지 않은경우 보석 착용 불가 
	UWeaponItem* weapon = Cast<UWeaponItem>(EquipList[0]);
	if(weapon == nullptr)
		return;

	if(weapon->GetEmptyJewelSlotCount() == 0)
		return;

	// 빈 보석 슬롯 검색 
	if(jewelSlotIdx < 0)
	{
		for(int i=0; weapon->GetJewelSlotCount(); ++i)
		{
			if(weapon->IsEmptyJewel(i))
			{
				jewelSlotIdx = i;
				break;
			}
		}
	}

	if(jewelSlotIdx < 0)
		return;

	if(weapon->EquipJewel(jewelSlotIdx, JewelItem))
	{
		RemoveItem(inventoryIdx);

		if (StatComponent != nullptr)
		{
			StatComponent->AddExtraMaxHp(JewelItem->MaxHp);
			StatComponent->AddExtraAtk(JewelItem->Atk);
			StatComponent->AddExtraDef(JewelItem->Def);
		}
	
		if(OnJewelSlotChange.IsBound())
		{
			OnJewelSlotChange.Execute(jewelSlotIdx, JewelItem);
		}
	}

}

void UInventoryComponent::UnEquipJewel(int32 jewelSlotIndex, int32 targetInventoryIdx)
{
	if (targetInventoryIdx < 0)
	{
		targetInventoryIdx = FindItemSlotIndex(-1);
	}

	// 무기가 없는경우 장착 해제 불가
	UWeaponItem* weapon = Cast<UWeaponItem>(EquipList[0]);
	if(weapon == nullptr)
		return;
	
	if (targetInventoryIdx < 0)
		return;
	if (jewelSlotIndex < 0)
		return;
	
	if (targetInventoryIdx >= ItemList.Num())
		return;
	if (jewelSlotIndex >= weapon->GetJewelSlotCount())
		return;

	if (ItemList[targetInventoryIdx] != nullptr)
		return;
	if(weapon->IsEmptyJewel(jewelSlotIndex))
		return;
	
	TObjectPtr<UJewelItem> temp = weapon->UnEquipJewel(jewelSlotIndex);
	if(temp == nullptr)
		return;

	ItemList[targetInventoryIdx] = temp;
	
	if (StatComponent != nullptr)
	{
		StatComponent->AddExtraMaxHp(-(temp->MaxHp));
		StatComponent->AddExtraAtk(-(temp->Atk));
		StatComponent->AddExtraDef(-(temp->Def));
	}
	
	if (OnItemSlotChanged.IsBound())
	{
		OnItemSlotChanged.Execute(targetInventoryIdx, ItemList[targetInventoryIdx], HighlightCategory);
	}

	if(OnJewelSlotChange.IsBound())
	{
		OnJewelSlotChange.Execute(jewelSlotIndex, nullptr);
	}
}


void UInventoryComponent::OnInventoryOpen()
{
	for(int i=0; i<ItemList.Num(); ++i)
	{
		if (OnItemSlotChanged.IsBound())
		{
			OnItemSlotChanged.Execute(i, ItemList[i], HighlightCategory);
		}

		if (ItemList[i] == nullptr)
			continue;

		ItemList[i]->UpdatedItem = false;
	}
}

void UInventoryComponent::SetHighlightItem(EItemCategory highlightCategory)
{
	HighlightCategory = highlightCategory;

	if (OnItemSlotChanged.IsBound())
	{
		for (int i = 0; i < ItemList.Num(); ++i)
		{
			OnItemSlotChanged.Execute(i, ItemList[i], HighlightCategory);
		}
	}
}

