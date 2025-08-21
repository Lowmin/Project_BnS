// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillIcon.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Animation/WidgetAnimation.h"

void USkillIcon::NativeConstruct()
{
	Super::NativeConstruct();

	UmatInstance = UMaterialInstanceDynamic::Create(UmatBase, this);
	Block->SetBrushFromMaterial(UmatInstance);

	TObjectPtr<UMaterialInstanceDynamic> PrevUmatInstance = UMaterialInstanceDynamic::Create(UmatBase, this);
	BlockPrev->SetBrushFromMaterial(PrevUmatInstance);

}

void USkillIcon::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (ChangeProgress > 1.0f)
		return;

	ChangeProgress = FMath::Clamp(ChangeProgress + (InDeltaTime * 1.0f), 0.0f, 1.0f);
	Icon->GetDynamicMaterial()->SetScalarParameterValue(TEXT("Progress"), ChangeProgress);
}

void USkillIcon::ChangeSkillIcon(UTexture2D* prevTexture, UTexture2D* texture)
{
	ChangeProgress = 1.0f;

	// 이전스킬 설정 
	IconPrev->GetDynamicMaterial()->SetTextureParameterValue(TEXT("PrevTexture"), prevTexture);
	float progress = 0.0f;
	FHashedMaterialParameterInfo info(TEXT("Progress"));
	Block->GetDynamicMaterial()->GetScalarParameterValue(info, progress);
	BlockPrev->GetDynamicMaterial()->SetScalarParameterValue(TEXT("Progress"), progress);
	CooldownTextPrev->SetVisibility(CooldownText->GetVisibility());
	CooldownTextPrev->SetText(CooldownText->GetText());

	// 현재스킬 설정 
	Icon->GetDynamicMaterial()->SetTextureParameterValue(TEXT("Texture"), texture);
	Icon->GetDynamicMaterial()->SetScalarParameterValue(TEXT("Progress"), ChangeProgress);

	PlayAnimation(AniSwap);
}

void USkillIcon::ChangeSkillIconStep(UTexture2D* prevTexture, UTexture2D* texture)
{
	ChangeProgress = 0.0f;

	Icon->GetDynamicMaterial()->SetTextureParameterValue(TEXT("PrevTexture"), prevTexture);
	Icon->GetDynamicMaterial()->SetTextureParameterValue(TEXT("Texture"), texture);
	Icon->GetDynamicMaterial()->SetScalarParameterValue(TEXT("Progress"), ChangeProgress);
}

void USkillIcon::SetIcon(UTexture2D* texture)
{
	Icon->GetDynamicMaterial()->SetTextureParameterValue(TEXT("Texture"), texture);
	ChangeProgress = 1.0f;
}

void USkillIcon::SetCooldown(float remain, float cooldown)
{
	if (UmatInstance == nullptr)
	{
		UE_LOG(LogActor, Warning, TEXT("umat null"));
		return;
	}

	int32 remainTime = FMath::CeilToInt32(remain);

	if (remainTime == 0)
	{
		Block->SetVisibility(ESlateVisibility::Hidden);
		CooldownText->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		Block->SetVisibility(ESlateVisibility::Visible);
		CooldownText->SetVisibility(ESlateVisibility::Visible);

		UmatInstance->SetScalarParameterValue(TEXT("Progress"), remain / cooldown);
		CooldownText->SetText(FText::AsNumber(remainTime));
	}
}
