// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GoldDisplay.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_BNS_API UGoldDisplay : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> TextGold = nullptr;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> ImgGold = nullptr;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> TextSilver= nullptr;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> ImgSilver= nullptr;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> TextCopper = nullptr;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> ImgCopper= nullptr;

public:
	void SetGold(int32 value, bool isFullDisplay = true);
};
