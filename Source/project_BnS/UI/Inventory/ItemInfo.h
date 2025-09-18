// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemInfo.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_BNS_API UItemInfo : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> TextItemName = nullptr;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> ImgIcon = nullptr;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> TextType= nullptr;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> TextOption = nullptr;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> TextDescript= nullptr;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UGoldDisplay> GoldDisplay = nullptr;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> TextCount = nullptr;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> TextDiff= nullptr;

public:
	void ShowInfo(const class UItem* data);
	void HideInfo();
};
