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

public:
	UItemInfo(const FObjectInitializer& ObjectInitializer);

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

	UPROPERTY()
	TObjectPtr<UTexture2D> TextureJewelSlot = nullptr;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UHorizontalBox> JewelRoot = nullptr;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UImage> JewelSlot_0 = nullptr;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UImage> JewelSlot_1 = nullptr;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UImage> JewelSlot_2 = nullptr;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UImage> JewelSlot_3 = nullptr;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UImage> JewelSlot_4 = nullptr;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UImage> JewelSlot_5 = nullptr;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UGoldDisplay> GoldDisplay = nullptr;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> TextCount = nullptr;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> TextDiff= nullptr;

public:
	void ShowInfo(const class UItem* data, const FString& diffText);
	void HideInfo();
	void SetJewelTexture(class UImage* image, class UJewelItem* jewelData);
};
