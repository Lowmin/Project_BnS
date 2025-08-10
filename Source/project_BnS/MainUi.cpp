// Fill out your copyright notice in the Description page of Project Settings.


#include "MainUi.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UMainUi::SetHp(float current, float max)
{
	HpBar->SetPercent(current / max);

	const FString formatString = FString::Printf(TEXT("%d/%d"), FMath::FloorToInt32(current), FMath::FloorToInt32(max));
	HpText->SetText(FText::FromString(formatString));
}

void UMainUi::SetStamina(float current, float max)
{
	StaminaBar->SetPercent(current / max);

	const FString formatString = FString::Printf(TEXT("%d/%d"), FMath::FloorToInt32(current), FMath::FloorToInt32(max));
	StaminaText->SetText(FText::FromString(formatString));
}

void UMainUi::SetStaminaEnable(bool enable)
{
	StaminaBar->SetFillColorAndOpacity(enable ? FLinearColor::Yellow : FLinearColor::Gray);
}

void UMainUi::SetExp(float current, float max)
{
	ExpBar->SetPercent(current / max);
}

void UMainUi::SetNickname(const FString& nickname)
{
	NicknameText->SetText(FText::FromString(nickname));
}

void UMainUi::SetLevel(int level)
{
	LevelText->SetText(FText::AsNumber(level));
}
