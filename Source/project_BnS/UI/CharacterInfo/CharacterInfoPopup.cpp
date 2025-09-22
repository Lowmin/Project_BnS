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
		FString LevelString = FString::Printf(TEXT("%d"), StatComponent->GetLevel());
		Text_Level->SetText(FText::FromString(LevelString));
	}

	if (Text_HP)
	{
		FString HPString = FString::Printf(TEXT("%.0f"), StatComponent->GetMaxHp());
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

	if (Text_TotalATK)
	{
		float TotalATK = StatComponent->GetAtk() * (1 + (10 * 100));
		FString TotalATKString = FString::Printf(TEXT("%.0f"), TotalATK);
		Text_TotalATK->SetText(FText::FromString(TotalATKString));
	}

	if (Text_TotalDEF)
	{
		float TotalDEF = StatComponent->GetMaxHp() + StatComponent->GetDef();
		FString TotalDEFString = FString::Printf(TEXT("%.0f"), TotalDEF);
		Text_TotalDEF->SetText(FText::FromString(TotalDEFString));
	}
}