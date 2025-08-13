// Fill out your copyright notice in the Description page of Project Settings.


#include "BossInfo.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UBossInfo::SetInfo(float curHp, float maxHp, int32 level, FString name, float distance)
{
	OnChangeHp(curHp, maxHp);
	SetLevel(level);
	SetName(name);
	SetDistance(distance);
}

void UBossInfo::OnChangeHp(float current, float max)
{
	float amount = current / max;
	HpBar->SetPercent(amount);

	int32 curHp = FMath::FloorToInt32(current);
	int32 maxHp = FMath::FloorToInt32(max);
	FString str = FString::Printf(TEXT("%d/%d"), curHp, maxHp);
	HpText->SetText(FText::FromString(str));

	int32 percent = FMath::FloorToInt32(amount * 100.f);
	FString strPercent = FString::Printf(TEXT("%d%%"), percent);
	HpPercentText->SetText(FText::FromString(strPercent));
	
}
void UBossInfo::SetLevel(int32 level)
{
	LevelText->SetText(FText::AsNumber(level));
}
void UBossInfo::SetName(FString name)
{
	NameText->SetText(FText::FromString(name));
}
void UBossInfo::SetDistance(float distance)
{
	int32 dist = FMath::CeilToInt32(distance);
	FString str = FString::Printf(TEXT("%d m"), dist);
	DIstanceText->SetText(FText::FromString(str));
}