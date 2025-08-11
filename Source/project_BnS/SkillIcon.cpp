// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillIcon.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"

void USkillIcon::NativeConstruct()
{
	UmatInstance = UMaterialInstanceDynamic::Create(UmatBase, this);
	Block->SetBrushFromMaterial(UmatInstance);
}

void USkillIcon::SetCooldown(float remain, float cooldown)
{
	if (UmatInstance == nullptr)
	{
		UE_LOG(LogActor, Warning, TEXT("umat null"));
		return;
	}

	int remainTime = FMath::CeilToInt32(remain);

	//if (remainTime == 0)
	{
	}
	//else
	{
		UmatInstance->SetScalarParameterValue(TEXT("Progress"), remain / cooldown);
		CooldownText->SetText(FText::AsNumber(remainTime));
	}
}
