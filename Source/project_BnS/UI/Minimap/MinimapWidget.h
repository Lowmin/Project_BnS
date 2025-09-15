// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MinimapWidget.generated.h"

class UImage;
class UMaterialInstanceDynamic;

/**
 *
 */
UCLASS()
class PROJECT_BNS_API UMinimapWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void SetMapPosition(const FVector2D& MapCenterUV);

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UImage> MinimapImage;

private:
	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> MinimapMaterialInst;
};
