// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EquipSlot.h"
#include "WeaponSlot.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_BNS_API UWeaponSlot : public UEquipSlot
{
	GENERATED_BODY()
	
protected:
	TArray<TObjectPtr<class UJewelSlot>> EquipJewelList;
	
public:
	void AddJewelSlot(class UJewelSlot* slot);
	TArray<TObjectPtr<class UJewelSlot>> GetJewelSlotList() const;
	virtual void SetInfo(const class UItem* data) override;
	void SetJewelSlot(int32 jewelSlotIndex, const class UJewelItem* data);
};
