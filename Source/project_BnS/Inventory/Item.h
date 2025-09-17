// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ItemData.h"
#include "Item.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_BNS_API UItem : public UObject
{
	GENERATED_BODY()

public:
	int32 Id = 0;
	EItemCategory Category = EItemCategory::Etc;
	EItemRarity Rarity = EItemRarity::Common;
	bool IsStackAble = false;
	int32 Count = 0;
	UPROPERTY()
	TObjectPtr<UTexture2D> Icon = nullptr;
	bool UpdatedItem = false;
	FString Description = "";

	void SetData(const FItemData* data);
	bool IsHighlight(EItemCategory category) const;
};
