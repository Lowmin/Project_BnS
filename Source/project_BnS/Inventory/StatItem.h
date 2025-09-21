// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Microsoft/AllowMicrosoftPlatformTypes.h"
#include "StatItem.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_BNS_API UStatItem : public UItem
{
	GENERATED_BODY()
	
public:
	float MaxHp = 0;
	float Atk = 0;
	float Def = 0;
	
	struct FStatItemDiffData GetDiffData(const UStatItem* diffItem) const;
};

USTRUCT()
struct FStatItemDiffData
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	int32 Atk;
	UPROPERTY(VisibleAnywhere)
	int32 Def;
	UPROPERTY(VisibleAnywhere)
	int32 MaxHp;

	FStatItemDiffData()
	{
		Atk = 0;
		Def = 0;
		MaxHp = 0;
	}
	FStatItemDiffData(const UStatItem* item) : FStatItemDiffData()
	{
		if(item == nullptr)
			return;
		
		Atk = item->Atk;
		Def = item->Def;
		MaxHp = item->MaxHp;
	}

	void operator+=(const FStatItemDiffData& other)
	{
		Atk += other.Atk;
		Def += other.Def;
		MaxHp += other.MaxHp;
	}
	void operator-=(const FStatItemDiffData& other)
	{
		Atk -= other.Atk;
		Def -= other.Def;
		MaxHp -= other.MaxHp;
	}

	void operator += (const UStatItem* item)
	{
		if(item == nullptr)
			return;
		
		Atk += item->Atk;
		Def += item->Def;
		MaxHp += item->MaxHp;
	}
	
	void operator -= (const UStatItem* item)
	{
		if(item == nullptr)
			return;
		
		Atk -= item->Atk;
		Def -= item->Def;
		MaxHp -= item->MaxHp;
	}

public:
	FString ToString() const
	{
		FString strDiff = "";
		
		if(Atk != 0)
		{
			strDiff += "Atk ";
			if(Atk > 0)
			{
				strDiff += "+";
			}
			strDiff += FString::Printf(TEXT("%d"), Atk);
		}
		if(Def != 0)
		{
			if(strDiff.Len() > 0)
				strDiff += "\n";
			strDiff += "Def ";
			if(Def > 0)
			{
				strDiff += "+";
			}
			strDiff += FString::Printf(TEXT("%d"), Def);
		}
		if(MaxHp != 0)
		{
			if(strDiff.Len() > 0)
				strDiff += "\n";
			strDiff += "MaxHp ";
			if(MaxHp > 0)
			{
				strDiff += "+";
			}
			strDiff += FString::Printf(TEXT("%d"), MaxHp);
		}
	
		return strDiff;
	}
};