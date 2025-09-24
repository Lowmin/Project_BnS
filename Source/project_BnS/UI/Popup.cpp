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

    if (OnClosePopup.IsBound())
    {
        OnClosePopup.Execute(this);
    }
}


void UPopup::SetPopupDesireInfo(FVector2D position, FVector2D size, bool isSetContentSize)
{
    PopupPosition = position;
    PopupSize = size;
    bIsSetContentSize = isSetContentSize;
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

FVector2D UPopup::GetPopupPosition() const
{
    return PopupPosition;
}

FVector2D UPopup::GetPopupSize() const
{
    return PopupSize;
}

bool UPopup::IsSetContentSize() const
{
    return bIsSetContentSize;
}
