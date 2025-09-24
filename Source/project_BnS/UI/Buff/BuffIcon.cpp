// Fill out your copyright notice in the Description page of Project Settings.


#include "BuffIcon.h"

#include "../../Buff/BuffData.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

UBuffIcon::UBuffIcon(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UMaterial> mat(TEXT("/Game/UI/umatProgressRadial.umatProgressRadial"));
	if(mat.Succeeded())
	{
		UmatBase = mat.Object;
	}
}

void UBuffIcon::NativeConstruct()
{
	Super::NativeConstruct();
	
	UmatInstance = UMaterialInstanceDynamic::Create(UmatBase, this);
	Block->SetBrushFromMaterial(UmatInstance);
}

void UBuffIcon::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Remain -= InDeltaTime;

	if (Remain < 0)
	{
		SetRemain(0, Duration);
		return;
	}

	SetRemain(Remain, Duration);
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

	BuffID = data.Idx;
	Duration = data.Duration;
	Remain = Duration;

	SetRemain(Remain, Duration);
}

int32 UBuffIcon::GetBuffID() const
{
	return BuffID;
}
