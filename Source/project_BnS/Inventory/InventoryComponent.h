// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "itemData.h"
#include "InventoryComponent.generated.h"

DECLARE_DELEGATE_ThreeParams(FDele_ItemSlotChange, int32, const class UItem*, const EItemCategory&);
DECLARE_DELEGATE_TwoParams(FDele_EquipSlotChange, int32, const class UItem*);
DECLARE_DELEGATE_TwoParams(FDele_SoulShieldSlotChange, int32, const class UItem*);


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
	/// 장비아이템 ID 한계 
	/// </summary>
	const int32 EQUIP_ID_LIMIT = 1000;
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
	void AddItem(int32 id, int32 count);
	void RemoveItem(int32 inventoryIdx);
	void UseItem(int32 inventoryIdx);
	bool IsEquipAbleSlot(int32 equipIdx) const;
	void Equip(int32 inventoryIdx, int32 equipIdx);
	/**
	 * @brief 장비 장착 
	 * @param inventoryIdx 장착 할 인벤트로 슬롯 인덱스 
	 * @param equipItem 장착 할 장비 정보 
	 */
	void Equip(int32 inventoryIdx, UEquipItem* equipItem);
	void UnEquip(int32 equipIdx, int32 targetInventoryIdx = -1);
	bool IsEquipAbleSoulShieldSlot(int32 equipIdx) const;
	void EquipSoulShield(int32 inventoryIdx, int32 soulShieldIdx);
	void EquipSoulShield(int32 inventoryIdx, USoulShieldItem* soulShieldItem);
	void UnEquipSoulShield(int32 soulShieldSlotIdx, int32 targetInventoryIdx = -1);
	void OnInventoryOpen();
	void SetHighlightItem(EItemCategory highlightCategory);
		
	FDele_ItemSlotChange OnItemSlotChanged;
	FDele_EquipSlotChange OnEquipSlotChanged;
	FDele_SoulShieldSlotChange OnSoulShieldSlotChanged;
};
