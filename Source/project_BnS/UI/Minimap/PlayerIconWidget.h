// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerIconWidget.generated.h"

class UImage;

/**
 *
 */
UCLASS()
class PROJECT_BNS_API UPlayerIconWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void UpdateRotation(float PlayerTargetAngle);

protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UImage> PlayerIconImage;

private:
	UPROPERTY(EditAnywhere, Category = "Minimap Settings")
	float RotationSpeed = 5.0f;

	float CurrentRotationAngle = 0.f;
	float TargetRotationAngle = 0.f;
};
