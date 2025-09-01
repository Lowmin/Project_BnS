// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SkillIcon.generated.h"

class UImage;
class UTextBlock;

/**
 * 
 */
UCLASS()
class PROJECT_BNS_API USkillIcon : public UUserWidget
{
	GENERATED_BODY()

private:
	float ChangeProgress = 1.0f;

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(EditDefaultsOnly)
	UMaterial* UmatBase = nullptr;

	UPROPERTY()
	UMaterialInstanceDynamic* UmatInstance = nullptr;

	// Current 
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UImage* Icon = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UImage* Block = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* CooldownText = nullptr;

	// Prev
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UImage* IconPrev = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UImage* BlockPrev = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* CooldownTextPrev = nullptr;
	
	// 스왑 애니메이션 
	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* AniSwap;
public:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void ChangeSkillIcon(UTexture2D* prevTexture, UTexture2D* texture);
	void ChangeSkillIconChain(UTexture2D* prevTexture, UTexture2D* texture);
	void SetIcon(UTexture2D* texture);
	void SetCooldown(float remain, float cooldown, bool isVisibleNum);
};
