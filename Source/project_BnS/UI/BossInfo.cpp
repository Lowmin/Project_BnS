// Fill out your copyright notice in the Description page of Project Settings.


#include "BossInfo.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/HorizontalBox.h"
#include "Components/Image.h"
#include "CrowdControlDisplay.h"
#include "../CrowdControlComponent.h"

void UBossInfo::NativeConstruct()
{
	Super::NativeConstruct();

	for (UWidget* child : CrowdControlRoot->GetAllChildren())
	{
		UCrowdControlDisplay* img = Cast<UCrowdControlDisplay>(child);
		if (img == nullptr)
			continue;

		ImgCCList.Add(img);
	}
}

void UBossInfo::SetInfo(float curHp, float maxHp, int32 level, FString name, int32 activateCCCount, float distance)
{
	OnChangeHp(curHp, maxHp);
	SetLevel(level);
	SetName(name);
	SetMaxCrowdControlCount(activateCCCount);
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
void UBossInfo::SetMaxCrowdControlCount(int32 activateCCCount)
{
	for (int i = 0; i < ImgCCList.Num(); ++i)
	{
		ImgCCList[i]->SetVisibility(i < activateCCCount ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
	}

	SetBossCCInfo(ECrowdControlType::Immune, 0);
}
void UBossInfo::SetBossCCInfo(ECrowdControlType type, int32 count)
{
	for (int i = 0; i < ImgCCList.Num(); ++i)
	{
		if (type == ECrowdControlType::Immune)
		{
			ImgCCList[i]->SetType(ECrowdControlType::Immune);
		}
		else if(i < count)
		{
			ImgCCList[i]->SetType(type);
		}
		else
		{
			ImgCCList[i]->SetType(ECrowdControlType::None);
		}
	}
}

void UBossInfo::SetDistance(float distance)
{
	int32 dist = FMath::CeilToInt32(distance);
	FString str = FString::Printf(TEXT("%d m"), dist);
	DIstanceText->SetText(FText::FromString(str));
}