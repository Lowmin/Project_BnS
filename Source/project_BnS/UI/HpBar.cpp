// Fill out your copyright notice in the Description page of Project Settings.


#include "HpBar.h"

#include "Components/ProgressBar.h"

void UHpBar::OnChangeHp(float current, float max)
{
	HpBar->SetPercent(current / max);
}

void UHpBar::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	float hpPercent = HpBar->GetPercent();
	float bgPercent = Bg->GetPercent();
	if (FMath::IsNearlyEqual(hpPercent, bgPercent))
		return;

	Bg->SetPercent(FMath::Lerp(bgPercent, hpPercent, InDeltaTime * 5.0f));
}
