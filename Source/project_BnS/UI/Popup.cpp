// Fill out your copyright notice in the Description page of Project Settings.


#include "Popup.h"

#include "Components/Button.h"
#include "GameFramework/PlayerController.h"

void UPopup::NativeOnInitialized()
{
    Super::NativeOnInitialized();
    
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
    }
    else
    {
        SetVisibility(ESlateVisibility::Hidden);
    }
}
