// Fill out your copyright notice in the Description page of Project Settings.


#include "Popup.h"

#include "Components/Button.h"
#include "GameFramework/PlayerController.h"

void UPopup::NativeConstruct()
{
    Super::NativeConstruct();
    BtnClose->OnClicked.AddDynamic(this, &UPopup::ClosePopup);

    bIsFocusable = true;
}

void UPopup::NativeDestruct()
{
    Super::NativeDestruct();

    BtnClose->OnClicked.RemoveAll(this);
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
    }
    else
    {
        SetVisibility(ESlateVisibility::Hidden);
    }
}
