// Fill out your copyright notice in the Description page of Project Settings.


#include "MainUi.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "SkillIcon.h"
#include "BossInfo.h"

void UMainUi::NativeConstruct()
{
	Super::NativeConstruct();

	BossInfo->SetVisibility(ESlateVisibility::Hidden);
}

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

void UMainUi::SetTarget(bool isTarget, FVector2D center, FVector2D size)
{
	if (TargetSlot == nullptr)
	{
		TargetSlot = Cast<UCanvasPanelSlot>(Target->Slot);
	}

	if (TargetSlot == nullptr)
		return;

	float viewportScale = 1.f;
	FVector2D viewportSize = FVector2D::UnitVector;

	TargetSlot->SetPosition(center);
	TargetSlot->SetSize(size);
}

void UMainUi::SetSkillCooldown(int index, float remain, float cooldown)
{
	switch (index)
	{
	case 0:
		Skill_0->SetCooldown(remain, cooldown);
		break;
	case 1:
		Skill_1->SetCooldown(remain, cooldown);
		break;
	case 2:
		Skill_2->SetCooldown(remain, cooldown);
		break;
	case 3:
		Skill_3->SetCooldown(remain, cooldown);
		break;
	case 4:
		Skill_4->SetCooldown(remain, cooldown);
		break;
	default:
		break;
	}
}
