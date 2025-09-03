// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemSlot.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_BNS_API UItemSlot : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UImage> ImgIcon;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UImage> ImgNewBadge;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UTextBlock> TextCount;

public:
};
