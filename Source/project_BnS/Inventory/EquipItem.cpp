// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipItem.h"

FString UEquipItem::GetItemTypeText() const
{
	switch (DetailCategory)
	{
	case EEquipDetailCategory::Weapon:
		return "Weapon";
	case EEquipDetailCategory::Ring:
		return "Ring";
	case EEquipDetailCategory::Earring:
		return "Earring";
	case EEquipDetailCategory::Neckless:
		return "Neckless";
	case EEquipDetailCategory::Bracelit:
		return "Bracelit";
	}

	return Super::GetItemTypeText();
}

FString UEquipItem::GetItemOptionText() const
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

void UEquipItem::SetData(const FEquipData* data)
{
	Super::SetData(data);

	DetailCategory = data->DetailCategory;
	MaxHp = data->MaxHp;
	Atk = data->Atk;
	Def = data->Def;
}
