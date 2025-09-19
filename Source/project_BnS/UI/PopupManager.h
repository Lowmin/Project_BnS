// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PopupManager.generated.h"

class UPopup;

// 팝업 창 필요시 추가
UENUM(BlueprintType)
enum class EPopupType : uint8
{
	None,
	Inventory,
	WorldMap
};

UCLASS()
class PROJECT_BNS_API UPopupManager : public UObject
{
	GENERATED_BODY()

public:
	void Initialize(APlayerController* InController);
	UPopup* RegisterPopup(EPopupType PopupType, TSubclassOf<UPopup> PopupClass);
	void TogglePopup(EPopupType PopupType);

	void ClosePopup();

private:
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
