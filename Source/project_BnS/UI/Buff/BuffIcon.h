// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BuffIcon.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_BNS_API UBuffIcon : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	float Duration = 0.0f;
	float Remain = 0.0f;

private:
	void SetRemain(float remain, float duration);

protected:
	UPROPERTY(EditDefaultsOnly)
	UMaterial* UmatBase = nullptr;

	UPROPERTY()
	UMaterialInstanceDynamic* UmatInstance = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UImage> Icon = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UImage> Block = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UTextBlock> CooldownText = nullptr;

public:
	void SetBuff(const struct FBuffData& data);
};
