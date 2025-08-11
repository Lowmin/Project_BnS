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
	void SetCooldown(float remain, float cooldown);
};
