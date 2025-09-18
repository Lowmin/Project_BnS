// Fill out your copyright notice in the Description page of Project Settings.


#include "SoulShieldItem.h"


FString USoulShieldItem::GetItemOptionText() const
{
	FString optionText = "";

	if (Atk > 0)
	{
		optionText += FString::Printf(TEXT("Atk %.0f"), Atk);
	}

	if (Def > 0)
	{
		if (optionText.Len() > 0)
			optionText += "\n";
		optionText += FString::Printf(TEXT("Def %.0f"), Def);
	}

	if (MaxHp > 0)
	{
		if (optionText.Len() > 0)
			optionText += "\n";
		optionText += FString::Printf(TEXT("MaxHp %.0f"), MaxHp);
	}

	return optionText;
}

void USoulShieldItem::SetData(const FSoulShieldData* data)
{
	Super::SetData(data);

	DetailCategory = data->DetailCategory;
	MaxHp = data->MaxHp;
	Atk = data->Atk;
	Def = data->Def;
	SetIndex = data->SetIndex;
	SoulShieldTexture = data->SoulShieldTexture;
}
