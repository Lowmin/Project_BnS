// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "itemData.h"
#include "InventoryComponent.generated.h"

DECLARE_DELEGATE_ThreeParams(FDele_Single_ItemSlotChange, int32, const class UItem*, const EItemCategory&);
DECLARE_DELEGATE_TwoParams(FDele_Single_EquipSlotChange, int32, const class UItem*);
DECLARE_DELEGATE_TwoParams(FDele_Single_SoulShieldSlotChange, int32, const class UItem*);
DECLARE_DELEGATE_TwoParams(FDele_Single_JewelSlotChange, int32 jewelSlotIndex, const class UJewelItem* data);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECT_BNS_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	// Inventory
private:
	TWeakObjectPtr<class UStatComponent> StatComponent = nullptr;

	EItemCategory HighlightCategory = EItemCategory::All;
	UPROPERTY()
	TArray<TObjectPtr<class UItem>> ItemList;
	UPROPERTY()
	TArray<TObjectPtr<class UEquipItem>> EquipList;
	UPROPERTY()
	TArray<TObjectPtr<class USoulShieldItem>> SoulShieldList;

	/// <summary>
	/// 무기 아이템 ID 한계 
	/// </summary>
	const int32 WEAPON_ID_LIMIT = 10000000;

	const int32 JEWEL_ID_START = 19000000;

	/// <summary>
	/// 장비아이템 ID 한계 
	/// </summary>
	const int32 EQUIP_ID_LIMIT = 100000000;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UDataTable> WeaponDataTable;
	TMap<int32, const FWeaponData*> WeaponDataMap;
	void ParsingWeaponData();
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UDataTable> JewelDataTable;
	TMap<int32, const FJewelData*> JewelDataMap;
	void ParsingJewelData();
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UDataTable> EquipDataTable;
	TMap<int32, const FEquipData*> EquipDataMap;
	void ParsingEquipData();

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UDataTable> SoulShieldDataTable;
	TMap<int32, const FSoulShieldData*> SoulShieldDataMap;
	void ParsingSoulShieldData();


private:
	/// <summary>
	/// 아이템 ID값과 같은 슬롯 인덱스를 찾음 
	/// </summary>
	/// <param name="itemId"> 아이템 ID </param>
	/// <returns> 동일한 슬롯 인덱스 반환, 동일한 슬롯이 없는경우 가장 앞번호의 빈 슬롯 인덱스 반환, 빈 슬롯도 없는경우 -1 리턴 </returns>
	int32 FindItemSlotIndex(int32 itemId) const;
	class UItem* CreateItem(int32 itemId) const;

public:
	void InventorySort();
	void SwapItem(int32 indexA, int32 indexB);
	void SetInventorySlotCount(int32 count);
	void AddItem(int32 id, int32 count = 1);
	void RemoveItem(int32 inventoryIdx);
	void UseItem(int32 inventoryIdx);
	bool IsEquipAbleSlot(int32 equipIdx) const;
	void Equip(int32 inventoryIdx, int32 equipIdx);
	/**
	 * @brief 장비 장착 
	 * @param inventoryIdx 장착 할 인벤트로 슬롯 인덱스 
	 * @param equipItem 장착 할 장비 정보 
	 */
	void Equip(int32 inventoryIdx, class UEquipItem* equipItem);
	void UnEquip(int32 equipIdx, int32 targetInventoryIdx = -1);
	bool IsEquipAbleSoulShieldSlot(int32 equipIdx) const;
	void EquipSoulShield(int32 inventoryIdx);
	void EquipSoulShield(int32 inventoryIdx, class USoulShieldItem* soulShieldItem);
	void UnEquipSoulShield(int32 soulShieldSlotIdx, int32 targetInventoryIdx = -1);
	void EquipJewel(int32 inventoryIdx, int32 jewelSlotIdx);
	void EquipJewel(int32 inventoryIdx, int32 jewelSlotIdx, class UJewelItem* JewelItem);
	void UnEquipJewel(int32 jewelSlotIndex, int32 targetInventoryIdx = -1);
	void OnInventoryOpen();
	void SetHighlightItem(EItemCategory highlightCategory);
		
	FDele_Single_ItemSlotChange OnItemSlotChanged;
	FDele_Single_EquipSlotChange OnEquipSlotChanged;
	FDele_Single_SoulShieldSlotChange OnSoulShieldSlotChanged;
	FDele_Single_JewelSlotChange OnJewelSlotChange;
};
