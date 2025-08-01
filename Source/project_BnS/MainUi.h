// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainUi.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_BNS_API UMainUi : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintImplementableEvent)
	void SetHp(float current, float max);

	UFUNCTION(BlueprintImplementableEvent)
	void SetMp(int current);

	UFUNCTION(BlueprintImplementableEvent)
	void SetStamina(float current, float max);

	UFUNCTION(BlueprintImplementableEvent)
	void SetStaminaEnable(bool enable);

	UFUNCTION(BlueprintImplementableEvent)
	void SetExp(float current, float max);

	UFUNCTION(BlueprintImplementableEvent)
	void SetNickname(const FString& nickname);

	UFUNCTION(BlueprintImplementableEvent)
	void SetLevel(int level);

	UFUNCTION(BlueprintImplementableEvent)
	void SetTarget(bool isTarget, FVector2D center, FVector2D size);
};
