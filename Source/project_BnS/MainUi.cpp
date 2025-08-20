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

	SkillIcons.Emplace(Skill_0);
	
	SkillIcons.Emplace(Skill_1);
	SkillIcons.Emplace(Skill_2);
	SkillIcons.Emplace(Skill_3);
	SkillIcons.Emplace(Skill_4);
}

void UMainUi::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	HpEffect(InDeltaTime);
}

void UMainUi::SetHp(float current, float max)
{
	HpBar->SetPercent(current / max);

	const FString formatString = FString::Printf(TEXT("%d/%d"), FMath::FloorToInt32(current), FMath::FloorToInt32(max));
	HpText->SetText(FText::FromString(formatString));
}

void UMainUi::HpEffect(float InDeltaTime)
{
	float hpPercent = HpBar->GetPercent();
	float bgPercent = HpBg->GetPercent();
	if (FMath::IsNearlyEqual(hpPercent, bgPercent))
		return;

	HpBg->SetPercent(FMath::Lerp(bgPercent, hpPercent, InDeltaTime * 5.0f));
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

void UMainUi::SetTargetBox(FVector2D center, FVector2D size)
{
	if (TargetSlot == nullptr)
	{
		TargetSlot = Cast<UCanvasPanelSlot>(Target->Slot);
	}

	if (TargetSlot == nullptr)
		return;

	TargetSlot->SetPosition(center);
	TargetSlot->SetSize(size);
}

void UMainUi::ChangeSkillIcon(int index, UTexture2D* prevTexture, UTexture2D* texture)
{
}

void UMainUi::ChangeSkillIconStep(int index, UTexture2D* prevTexture, UTexture2D* texture)
{
	SkillIcons[index]->ChangeSkillIconStep(prevTexture, texture);
}

void UMainUi::SetSkillIcon(int index, UTexture2D* texture)
{
	SkillIcons[index]->SetIcon(texture);
}

void UMainUi::SetSkillCooldown(int index, float remain, float cooldown)
{
	SkillIcons[index]->SetCooldown(remain, cooldown);
}
