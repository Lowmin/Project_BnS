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

	// Input
public:
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent);
	virtual FReply NativeOnKeyUp(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent);

protected:
	FKey SelfShortCut = EKeys::Escape;
	
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> BtnClose = nullptr;

public:
	virtual void NativeConstruct() override;
	static int PopupCount;

	UFUNCTION()
	void ClosePopup();

	void SetVisiblePopup(bool isVisible);
};
