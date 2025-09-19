// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StatItem.h"
#include "SoulShieldItem.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_BNS_API USoulShieldItem : public UStatItem
{
	GENERATED_BODY()

public:
	virtual FString GetItemOptionText() const override;

public:
	ESoulShieldDetailCategory DetailCategory = ESoulShieldDetailCategory::SoulShield_0;
	int32 SetIndex = 0;
	UPROPERTY()
	TObjectPtr<UTexture2D> SoulShieldTexture = nullptr;

public:
	void SetData(const FSoulShieldData* data);
};
