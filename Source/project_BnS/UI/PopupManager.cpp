// Fill out your copyright notice in the Description page of Project Settings.


#include "PopupManager.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerController.h"
#include "Popup.h"

void UPopupManager::Initialize(APlayerController* InController)
{
	MyController = InController;
}

UPopup* UPopupManager::RegisterPopup(EPopupType PopupType, TSubclassOf<UPopup> PopupClass)
{
	if (!MyController.IsValid() || !PopupClass || PopupType == EPopupType::None || RegisteredPopup.Contains(PopupType)) return nullptr;

	UPopup* NewPopup = CreateWidget<UPopup>(MyController.Get(), PopupClass, FName(*UEnum::GetValueAsString(PopupType)));
	if (NewPopup)
	{
		NewPopup->AddToViewport();
		NewPopup->SetVisibility(ESlateVisibility::Hidden);
		RegisteredPopup.Add(PopupType, NewPopup);
		return NewPopup;
	}

	return nullptr;
}

void UPopupManager::TogglePopup(EPopupType PopupType)
{
	if (!RegisteredPopup.Contains(PopupType)) return;

	UPopup* PopupToToggle = RegisteredPopup[PopupType];
	if (!PopupToToggle) return;

	bool bIsNowVisible = !PopupToToggle->IsVisible();
	PopupToToggle->SetVisiblePopup(bIsNowVisible);

	if (bIsNowVisible)
	{
		VisiblePopupStack.AddUnique(PopupToToggle);
		PopupToToggle->RemoveFromParent();
		PopupToToggle->AddToViewport(++CurrentTopZOrder);
	}
	else
	{
		VisiblePopupStack.Remove(PopupToToggle);
	}

	SetInputMode();
}

bool UPopupManager::IsPopupVisible()
{
	return VisiblePopupStack.Num() > 0;
}

void UPopupManager::ClosePopup()
{
	if (VisiblePopupStack.Num() == 0) return;

	TArray<TWeakObjectPtr<UPopup>> PopupClose = VisiblePopupStack;
	for (TWeakObjectPtr<UPopup> Popup : PopupClose)
	{
		if (Popup.IsValid())
		{
			Popup->SetVisiblePopup(false);
		}
	}

	VisiblePopupStack.Empty();
	SetInputMode();
}

void UPopupManager::SetInputMode()
{
	if (!MyController.IsValid())
	{
		return;
	}

	const bool bIsPopupVisible = VisiblePopupStack.Num() > 0;

	if (bIsPopupVisible)
	{
		FInputModeGameAndUI InputMode;

		if (VisiblePopupStack.Last().IsValid())
		{
			InputMode.SetWidgetToFocus(VisiblePopupStack.Last()->TakeWidget());
		}

		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockOnCapture);
		MyController->SetInputMode(InputMode);
	}
	else
	{
		MyController->SetInputMode(FInputModeGameOnly());
	}

	MyController->SetShowMouseCursor(bIsPopupVisible);
}
