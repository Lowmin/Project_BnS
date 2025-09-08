// Fill out your copyright notice in the Description page of Project Settings.


#include "Popup.h"

#include "Components/Button.h"
#include "GameFramework/PlayerController.h"

int UPopup::PopupCount = 0;


void UPopup::NativeConstruct()
{
	BtnClose->OnClicked.AddDynamic(this, &UPopup::ClosePopup);
}

void UPopup::ClosePopup()
{
	SetVisiblePopup(false);
}

void UPopup::SetVisiblePopup(bool isVisible)
{
	if (isVisible)
	{
		SetVisibility(ESlateVisibility::Visible);
		++PopupCount;
	}
	else
	{
		SetVisibility(ESlateVisibility::Hidden);
		--PopupCount;
	}

	APlayerController* playerCon = GetWorld()->GetFirstPlayerController();
	if (playerCon == nullptr)
		return;

	// 인풋 모드 설정 
	if (UPopup::PopupCount > 0)
	{
		playerCon->SetInputMode(FInputModeGameAndUI());
	}
	else
	{
		playerCon->SetInputMode(FInputModeGameOnly());
	}

	// 마우스 커서 제어 
	playerCon->SetShowMouseCursor(UPopup::PopupCount > 0);
}
