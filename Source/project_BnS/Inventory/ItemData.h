// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemData.generated.h"

UENUM(BlueprintType)
enum class EItemCategory : uint8 
{
	Equip,
	SoulShield,
	Usable,
	Material,
	Etc,
	All
};

UENUM(BlueprintType)
enum class EItemRarity : uint8 
{
	Common,
	UnCommon,
	Rare,
	Legendary
};

UENUM(BlueprintType)
enum class EEquipDetailCategory : uint8 
{
	Weapon,
	Ring,
	Earring,
	Neckless,
	Bracelit,
	Count
};

UENUM(BlueprintType)
enum class ESoulShieldDetailCategory : uint8
{
	SoulShield_0,
	SoulShield_1,
	SoulShield_2,
	SoulShield_3,
	SoulShield_4,
	SoulShield_5,
	SoulShield_6,
	SoulShield_7,
};

USTRUCT(BlueprintType)
struct FItemData : public FTableRowBase 
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	int32 Id;
	UPROPERTY(VisibleAnywhere)
	FString ItemName;
	UPROPERTY(VisibleAnywhere)
	EItemCategory Category;
	UPROPERTY(VisibleAnywhere)
	EItemRarity Rarity;
	UPROPERTY(VisibleAnywhere)
	bool IsStackAble;
	UPROPERTY(VisibleAnywhere)
	UTexture2D* Icon;
	UPROPERTY(VisibleAnywhere)
	FString Description;
	UPROPERTY(VisibleAnywhere)
	int32 Price;

	FItemData()
	{
		Id = 0;
		ItemName = "";
		Category = EItemCategory::Etc;
		Rarity = EItemRarity::Common;
		IsStackAble = false;
		Icon = nullptr;
		Description = "";
		Price = 0;
	};
};


USTRUCT(BlueprintType)
struct FEquipData : public FItemData 
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	EEquipDetailCategory DetailCategory;
	UPROPERTY(VisibleAnywhere)
	float MaxHp;
	UPROPERTY(VisibleAnywhere)
	float Atk;
	UPROPERTY(VisibleAnywhere)
	float Def;

	FEquipData() : FItemData()
	{
		DetailCategory = EEquipDetailCategory::Weapon;
		MaxHp = 0.f;
		Atk = 0.f;
		Def = 0.f;
	};
};

USTRUCT(BlueprintType)
struct FSoulShieldData : public FItemData 
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	ESoulShieldDetailCategory DetailCategory;

	UPROPERTY(VisibleAnywhere)
	float MaxHp;
	UPROPERTY(VisibleAnywhere)
	float Atk;
	UPROPERTY(VisibleAnywhere)
	float Def;
	UPROPERTY(VisibleAnywhere)
	int32 SetIndex;

	UPROPERTY(VisibleAnywhere)
	UTexture2D* SoulShieldTexture;

	FSoulShieldData() : FItemData()
	{
		DetailCategory = ESoulShieldDetailCategory::SoulShield_0;
		MaxHp = 0.f;
		Atk = 0.f;
		Def = 0.f;
		SetIndex = 0;
		SoulShieldTexture = nullptr;
	}
};