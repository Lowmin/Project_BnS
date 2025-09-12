// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemData.generated.h"

UENUM(BlueprintType)
enum class EItemCategory : uint8 {
	Equip,
	SoulShield,
	Usable,
	Material,
	Etc
};

UENUM(BlueprintType)
enum class EItemRarity : uint8 {
	Common,
	UnCommon,
	Rare,
	Legendary
};

UENUM(BlueprintType)
enum class EEquipDetailCategory : uint8 {
	Weapon,
	Armor,
	Ring,
	EarRing,
	Count
};


USTRUCT(BlueprintType)
struct FItemData : public FTableRowBase 
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	int32 Id;
	UPROPERTY(EditAnywhere)
	EItemCategory Category;
	UPROPERTY(EditAnywhere)
	EItemRarity Rarity;
	UPROPERTY(EditAnywhere)
	bool IsStackAble;
	UPROPERTY(EditAnywhere)
	UTexture2D* Icon;
	UPROPERTY(EditAnywhere)
	bool UpdatedItem;
	UPROPERTY(EditAnywhere)
	FString Description;

	FItemData()
	{
		Id = 0;
		Category = EItemCategory::Etc;
		Rarity = EItemRarity::Common;
		IsStackAble = false;
		Icon = nullptr;
		UpdatedItem = false;
		Description = "";
	};
};


USTRUCT(BlueprintType)
struct FEquipData : public FItemData {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	EEquipDetailCategory DetailCategory;
	UPROPERTY(EditAnywhere)
	float Hp;
	UPROPERTY(EditAnywhere)
	float Atk;
	UPROPERTY(EditAnywhere)
	float Def;

	FEquipData() : FItemData()
	{
		DetailCategory = EEquipDetailCategory::Weapon;
		Hp = 0.f;
		Atk = 0.f;
		Def = 0.f;
	};
};