// Fill out your copyright notice in the Description page of Project Settings.


#include "HpBar.h"

#include "Components/ProgressBar.h"

void UHpBar::OnChangeHp(float current, float max)
{
	HpBar->SetPercent(current / max);
}
