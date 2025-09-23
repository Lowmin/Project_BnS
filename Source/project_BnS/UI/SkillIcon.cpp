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

	ChangeProgress = FMath::Clamp(ChangeProgress + (InDeltaTime * 4.0f), 0.0f, 1.0f);

	// 사용 불가인 경우 프로그레스 변경하지 않음 
	if (!bIsUsable)
		return;

	Icon->GetDynamicMaterial()->SetScalarParameterValue(TEXT("Progress"), ChangeProgress);
}

void USkillIcon::ChangeSkillIcon(UTexture2D* prevTexture, UTexture2D* texture)
{
	ChangeProgress = 1.0f;

	// 이전스킬 설정 
	IconPrev->GetDynamicMaterial()->SetTextureParameterValue(TEXT("Texture"), prevTexture);
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

void USkillIcon::ChangeSkillIconChain(UTexture2D* prevTexture, UTexture2D* texture)
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
	Icon->GetDynamicMaterial()->SetScalarParameterValue(TEXT("Progress"), ChangeProgress);
}

void USkillIcon::SetCooldown(float remain, float cooldown, bool isVisibleNum)
{
	if (UmatInstance == nullptr)
	{
		UE_LOG(LogActor, Warning, TEXT("umat null"));
		return;
	}

	int32 remainTime = FMath::CeilToInt32(remain);

	if (isVisibleNum && remainTime > 0)
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
		UmatInstance->SetScalarParameterValue(TEXT("Progress"), remain / cooldown);
	}

	Block->SetVisibility((bIsUsable && remainTime == 0) ? ESlateVisibility::Hidden : ESlateVisibility::Visible);
}

void USkillIcon::SetSkillUsable(bool isUsable)
{
	bIsUsable = isUsable;

	Block->SetVisibility(bIsUsable ? ESlateVisibility::Hidden : ESlateVisibility::Visible);
	if(!bIsUsable)
	{
		UmatInstance->SetScalarParameterValue(TEXT("Progress"), 1.0f);
	}
}
