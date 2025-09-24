// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Popup.generated.h"

DECLARE_DELEGATE_OneParam(FDele_Single_ClosePopup, UPopup* popup);

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

	FVector2D PopupPosition = FVector2D::ZeroVector;
	FVector2D PopupSize = FVector2D::ZeroVector;
	bool bIsSetContentSize = false;

public:
	virtual void NativeOnInitialized() override;


	FDele_Single_ClosePopup OnClosePopup;

	UFUNCTION()
	void ClosePopup();

	void SetPopupDesireInfo(FVector2D position, FVector2D size, bool isSetContentSize);
	virtual void SetVisiblePopup(bool isVisible);

	FVector2D GetPopupPosition() const;
	FVector2D GetPopupSize() const;
	bool IsSetContentSize() const;

};
