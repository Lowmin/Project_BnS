// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PopupManager.generated.h"

class UPopup;

// 팝업 창 필요시 추가
UENUM(BlueprintType)
enum class EPopupType : uint8
{
	None,
	Inventory,
	WorldMap,
	CharacterInfo
};

UCLASS()
class PROJECT_BNS_API UPopupManager : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UCanvasPanel> Root = nullptr;

public:
	void Initialize(APlayerController* InController);
	UPopup* RegisterPopup(EPopupType PopupType, TSubclassOf<UPopup> PopupClass, FVector2D Position, FVector2D Size, bool bIsSetContentSize);
	void TogglePopup(EPopupType PopupType);
	bool IsPopupVisible();
	void ClosePopup();

private:
	void OnPopupClosed(UPopup* popup);
	void SetInputMode();

private:
	UPROPERTY()
	TMap<EPopupType, TObjectPtr<UPopup>> RegisteredPopup;

	UPROPERTY()
	TArray<TWeakObjectPtr<UPopup>> VisiblePopupStack;

	UPROPERTY()
	TWeakObjectPtr<APlayerController> MyController;

	int32 CurrentTopZOrder = 10;
};
