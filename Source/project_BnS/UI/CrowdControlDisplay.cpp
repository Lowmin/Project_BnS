// Fill out your copyright notice in the Description page of Project Settings.


#include "CrowdControlDisplay.h"

#include "Components/Image.h"
#include "../CrowdControlComponent.h"

UCrowdControlDisplay::UCrowdControlDisplay(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UTexture2D> textureNone(TEXT("/Game/Image/UI/CC_None.CC_None"));
	if (textureNone.Succeeded())
	{
		TextureNone = textureNone.Object;
	}
	static ConstructorHelpers::FObjectFinder<UTexture2D> textureDown(TEXT("/Game/Image/UI/CC_Down.CC_Down"));
	if (textureDown.Succeeded())
	{
		TextureDown = textureDown.Object;
	}
	static ConstructorHelpers::FObjectFinder<UTexture2D> textureStun(TEXT("/Game/Image/UI/CC_Stun.CC_Stun"));
	if (textureStun.Succeeded())
	{
		TextureStun = textureStun.Object;
	}
	static ConstructorHelpers::FObjectFinder<UTexture2D> textureImmune(TEXT("/Game/Image/UI/CC_Immune.CC_Immune"));
	if (textureImmune.Succeeded())
	{
		TextureImmune = textureImmune.Object;
	}
}

void UCrowdControlDisplay::NativeConstruct()
{
	Super::NativeConstruct();

	CurrentType = ECrowdControlType::Immune;
}

void UCrowdControlDisplay::SetType(ECrowdControlType type)
{
	if (CurrentType == type)
		return;

	CurrentType = type;

	switch (type)
	{
	case ECrowdControlType::None:
		Image->SetBrushFromTexture(TextureNone);
		break;
	case ECrowdControlType::Down:
		Image->SetBrushFromTexture(TextureDown);
		break;
	case ECrowdControlType::Stun:
		Image->SetBrushFromTexture(TextureStun);
		break;
	case ECrowdControlType::Immune:
		Image->SetBrushFromTexture(TextureImmune);
		break;
	}
}
