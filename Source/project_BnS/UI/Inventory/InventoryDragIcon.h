// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryDragIcon.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_BNS_API UInventoryDragIcon : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> ImgIcon = nullptr;

public:
	UInventoryDragIcon* SetIcon(UTexture2D* texture);
};
