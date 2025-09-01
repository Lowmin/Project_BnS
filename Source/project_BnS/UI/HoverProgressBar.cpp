// Fill out your copyright notice in the Description page of Project Settings.


#include "HoverProgressBar.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"


void UHoverProgressBar::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

	ProgressText->SetVisibility(ESlateVisibility::Visible);
}

void UHoverProgressBar::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	ProgressText->SetVisibility(ESlateVisibility::Hidden);
}

void UHoverProgressBar::SetPercent(float percent)
{
	ProgressBar->SetPercent(percent);
}

void UHoverProgressBar::SetText(const FText& text)
{
	ProgressText->SetText(text);
}
