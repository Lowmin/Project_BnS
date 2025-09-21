// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterInfoPopup.h"
#include "../../CharacterBase.h"
#include "Components/TextBlock.h"

void UCharacterInfoPopup::SetVisiblePopup(bool isVisible)
{
	Super::SetVisiblePopup(isVisible);

	if (isVisible)
	{
		UpdateCharacterInfo();
	}
}

void UCharacterInfoPopup::UpdateCharacterInfo()
{
	ACharacterBase* PlayerCharacter = Cast<ACharacterBase>(GetOwningPlayerPawn());
	if (!PlayerCharacter) return;

	UStatComponent* StatComponent = PlayerCharacter->GetStatusComponent();
	if (!StatComponent) return;

	if (Text_CharacterName)
	{
		Text_CharacterName->SetText(FText::FromString(StatComponent->GetCharacterName()));
	}

	if (Text_Level)
	{
		FString LevelString = FString::Printf(TEXT("Lv. %d"), StatComponent->GetLevel());
		Text_Level->SetText(FText::FromString(LevelString));
	}

	if (Text_HP)
	{
		FString HPString = FString::Printf(TEXT("%.0f / %.0f"), StatComponent->GetCurHp(), StatComponent->GetMaxHp());
		Text_HP->SetText(FText::FromString(HPString));
	}

	if (Text_ATK)
	{
		FString AttackString = FString::Printf(TEXT("%.0f"), StatComponent->GetAtk());
		Text_ATK->SetText(FText::FromString(AttackString));
	}

	if (Text_DEF)
	{
		FString DefenseString = FString::Printf(TEXT("%.0f"), StatComponent->GetDef());
		Text_DEF->SetText(FText::FromString(DefenseString));
	}
}