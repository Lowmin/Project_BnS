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

USTRUCT(BlueprintType)
struct FItemData : public FTableRowBase {
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
	int32 Count;
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
		Count = 0;
		Icon = nullptr;
		UpdatedItem = false;
		Description = "";
	};
};