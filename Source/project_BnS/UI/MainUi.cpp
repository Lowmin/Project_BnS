// Fill out your copyright notice in the Description page of Project Settings.


#include "MainUi.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/HorizontalBox.h"
#include "MpIcon.h"
#include "SkillIcon.h"
#include "BossInfo.h"
#include "../StatComponent.h"
#include "HoverProgressBar.h"

void UMainUi::NativeConstruct()
{
	Super::NativeConstruct();

	TArray<UWidget*> mpWidgets = MpSlot->GetAllChildren();
	for (UWidget* widget : mpWidgets)
	{
		MpList.Add(Cast<UMpIcon>(widget));
	}

	BossInfo->SetVisibility(ESlateVisibility::Hidden);

	SkillIcons.Emplace(Skill_0);
	
	SkillIcons.Emplace(Skill_1);
	SkillIcons.Emplace(Skill_2);
	SkillIcons.Emplace(Skill_3);
	SkillIcons.Emplace(Skill_4);

	TargetSlot = Cast<UCanvasPanelSlot>(Target->Slot);
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

void UMainUi::SetMp(int32 current)
{
	for (int i = 0; i < MpList.Num(); ++i)
	{
		MpList[i]->SetIconVisible(i < current);
	}
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
	int32 curExp = FMath::FloorToInt32(current);
	int32 maxExp = FMath::FloorToInt32(max);

	ExpBar->SetPercent(current / max);
	ExpBar->SetText(FText::FromString(FString::Printf(TEXT("%d/%d"), curExp, maxExp)));
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
		return;

	TargetSlot->SetPosition(center);
	TargetSlot->SetSize(size);	
}

void UMainUi::ChangeSkillIcon(int index, UTexture2D* prevTexture, UTexture2D* texture)
{
	SkillIcons[index]->ChangeSkillIcon(prevTexture, texture);
}

void UMainUi::ChangeSkillIconChain(int index, UTexture2D* prevTexture, UTexture2D* texture)
{
	SkillIcons[index]->ChangeSkillIconChain(prevTexture, texture);
}

void UMainUi::SetSkillIcon(int index, UTexture2D* texture)
{
	SkillIcons[index]->SetIcon(texture);
}

void UMainUi::SetSkillCooldown(int index, float remain, float cooldown, bool isVisibleNum)
{
	SkillIcons[index]->SetCooldown(remain, cooldown, isVisibleNum);
}

void UMainUi::SetBossInfo(const UStatComponent* status, float distance)
{
	if (status == nullptr)
	{
		BossInfo->SetVisibility(ESlateVisibility::Hidden);
		return;
	}

	BossInfo->SetVisibility(ESlateVisibility::Visible);
	BossInfo->SetInfo(status->GetCurHp(), status->GetMaxHp(), status->GetLevel(), status->GetCharacterName(), distance);
}

void UMainUi::SetBossHp(float current, float max)
{
	BossInfo->OnChangeHp(current, max);
}

void UMainUi::SetBossCrowdControlCount(int current, int max)
{
}

void UMainUi::SetBossDistance(float distance)
{
	BossInfo->SetDistance(distance);
}

UMinimapWidget* UMainUi::GetMinimapWidget() const
{
	return MinimapView;
}

UPlayerIconWidget* UMainUi::GetPlayerIconWidget() const
{
	return PlayerIconView;
}
