// Fill out your copyright notice in the Description page of Project Settings.


#include "GoldDisplay.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void UGoldDisplay::SetGold(int32 value, bool isFullDisplay)
{
	int32 gold = value / 10000;
	int32 silver = (value % 10000) / 100;
	int32 copper = value % 100;

	TextGold->SetText(FText::AsNumber(gold));
	TextSilver->SetText(FText::AsNumber(silver));
	TextCopper->SetText(FText::AsNumber(copper));

	TextGold->SetVisibility((isFullDisplay || gold > 0) ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	ImgGold->SetVisibility(TextGold->GetVisibility());
	TextSilver->SetVisibility((isFullDisplay || silver > 0) ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	ImgSilver->SetVisibility(TextSilver->GetVisibility());
	TextCopper->SetVisibility((isFullDisplay || copper > 0) ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	ImgCopper->SetVisibility(TextCopper->GetVisibility());
}
