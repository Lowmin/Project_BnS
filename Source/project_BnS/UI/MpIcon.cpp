// Fill out your copyright notice in the Description page of Project Settings.


#include "MpIcon.h"

#include "Components/Image.h"

void UMpIcon::SetIconVisible(bool visible)
{
	Icon->SetVisibility(visible ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}
