// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HoverProgressBar.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_BNS_API UHoverProgressBar : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UProgressBar> ProgressBar;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> ProgressText;

	
public:
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

	void SetPercent(float percent);
	void SetText(const FText& text);
};
