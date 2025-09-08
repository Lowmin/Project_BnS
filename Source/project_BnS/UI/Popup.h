// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Popup.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_BNS_API UPopup : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> BtnClose = nullptr;

public:
	//UPopup(const FObjectInitializer& ObjectInitializer);
	virtual void NativeConstruct() override;
	static int PopupCount;

	UFUNCTION()
	void ClosePopup();

	void SetVisiblePopup(bool isVisible);
};
