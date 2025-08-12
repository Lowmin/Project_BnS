// Fill out your copyright notice in the Description page of Project Settings.


#include "Nameplate.h"

#include "Components/TextBlock.h"

void UNameplate::SetNameplate(const FString name)
{
	NameText->SetText(FText::FromString(name));
}
