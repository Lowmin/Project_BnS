// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

DECLARE_DELEGATE_TwoParams(FDele_ItemSlotChange, int32, const struct FItemData&);


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
	TArray<struct FItemData> ItemList;
	TArray<struct FItemData> EquipList;
	TArray<struct FItemData> SoulSHieldList;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UDataTable> ItemDataTable;

	TMap<int32, struct FItemData*> DataMap;
	void ParsingData();

private:
	/// <summary>
	/// 아이템 ID값과 같은 슬롯 인덱스를 찾음 
	/// </summary>
	/// <param name="itemId"> 아이템 ID </param>
	/// <returns> 동일한 슬롯 인덱스 반환, 동일한 슬롯이 없는경우 가장 앞번호의 빈 슬롯 인덱스 반환, 빈 슬롯도 없는경우 -1 리턴 </returns>
	int32 FindItemSlotIndex(int32 itemId) const;

	const struct FItemData* GetItemData(int32 itemId) const;

public:
	void SetInventorySlotCount(int32 count);
	void AddItem(int32 id, int32 count);
	void RemoveItem(int32 inventoryIdx);
	void UseItem(int32 inventoryIdx);
	void Unequip(int32 equipIdx);
	void UnequipSoulShield(int32 soulShieldSlotIdx);
		
	FDele_ItemSlotChange OnItemSlotChanged;
};
