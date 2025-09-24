// Fill out your copyright notice in the Description page of Project Settings.


#include "BuffIcon.h"

#include "../../Buff/BuffData.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UBuffIcon::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Green, "Tick");

	Remain -= InDeltaTime;

	if (Remain < 0)
	{
		return;
	}


}

void UBuffIcon::SetRemain(float remain, float duration)
{
	int32 remainTime = FMath::CeilToInt32(remain);

	if (remainTime > 0)
	{
		CooldownText->SetVisibility(ESlateVisibility::Visible);
		CooldownText->SetText(FText::AsNumber(remainTime));
	}
	else
	{
		CooldownText->SetVisibility(ESlateVisibility::Hidden);
	}

	if (remainTime == 0)
	{
	}
	else
	{
		UmatInstance->SetScalarParameterValue(TEXT("Progress"), remain / duration);
	}
}

void UBuffIcon::SetBuff(const FBuffData& data)
{
	Icon->GetDynamicMaterial()->SetTextureParameterValue(TEXT("Texture"), data.Icon);


	Duration = data.Duration;
	Remain = Duration;

	SetRemain(Remain, Duration);
		
}
