// Fill out your copyright notice in the Description page of Project Settings.


#include "Popup.h"

#include "Components/Button.h"
#include "GameFramework/PlayerController.h"

int UPopup::PopupCount = 0;


FReply UPopup::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	auto  res = Super::NativeOnKeyDown(InGeometry, InKeyEvent);
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, "pop OnkeyDown");
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, InKeyEvent.ToText().ToString());

	FKey Key = InKeyEvent.GetKey();

	if (Key == EKeys::W)
	{
		ClosePopup();
	}else if (Key == EKeys::A)
	{
		ClosePopup();
	}
	else if (Key == EKeys::S)
	{
		ClosePopup();
	}
	else if (Key == EKeys::D)
	{
		ClosePopup();
	}
	else if (Key == EKeys::SpaceBar)
	{
		ClosePopup();
	}
	else if (Key == EKeys::Escape)
	{
		ClosePopup();
	}

	return res;
}

FReply UPopup::NativeOnKeyUp(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	auto  res = Super::NativeOnKeyDown(InGeometry, InKeyEvent);
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, "pop Onkeyup");
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, InKeyEvent.ToText().ToString());

	if (InKeyEvent.GetKey() == SelfShortCut)
		ClosePopup();

	return res;
}

void UPopup::NativeConstruct()
{
	BtnClose->OnClicked.AddDynamic(this, &UPopup::ClosePopup);

	bIsFocusable = true;
}

void UPopup::ClosePopup()
{
	SetVisiblePopup(false);
}

void UPopup::SetVisiblePopup(bool isVisible)
{
	if (isVisible)
	{
		SetKeyboardFocus();
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
		playerCon->SetInputMode(FInputModeUIOnly());
	}
	else
	{
		playerCon->SetInputMode(FInputModeGameOnly());
	}

	// 마우스 커서 제어 
	playerCon->SetShowMouseCursor(UPopup::PopupCount > 0);
}
