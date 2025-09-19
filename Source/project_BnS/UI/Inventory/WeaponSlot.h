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
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UEquipSlot> EquipJewel_0 = nullptr;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UEquipSlot> EquipJewel_1 = nullptr;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UEquipSlot> EquipJewel_2 = nullptr;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UEquipSlot> EquipJewel_3 = nullptr;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UEquipSlot> EquipJewel_4 = nullptr;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UEquipSlot> EquipJewel_5 = nullptr;

public:
	virtual void SetInfo(const class UItem* data) override;
};
