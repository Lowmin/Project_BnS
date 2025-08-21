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
	float ChangeProgress = 0.0f;
	bool IsStepEffect = false;

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UImage* Icon = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UImage* Block = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* CooldownText = nullptr;

	UPROPERTY(EditDefaultsOnly)
	UMaterial* UmatBase = nullptr;

	UPROPERTY()
	UMaterialInstanceDynamic* UmatInstance = nullptr;
public:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void ChangeSkillIcon(UTexture2D* prevTexture, UTexture2D* texture);
	void ChangeSkillIconStep(UTexture2D* prevTexture, UTexture2D* texture);
	void SetIcon(UTexture2D* texture);
	void SetCooldown(float remain, float cooldown);
};
