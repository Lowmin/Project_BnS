// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainUi.generated.h"


class UProgressBar;
class UTextBlock;

/**
 * 
 */
UCLASS()
class PROJECT_BNS_API UMainUi : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UProgressBar* HpBar = nullptr;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* HpText = nullptr;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UProgressBar* StaminaBar = nullptr;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* StaminaText = nullptr;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UProgressBar* ExpBar = nullptr;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* NicknameText = nullptr;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* LevelText = nullptr;
	
public:
	UFUNCTION(BlueprintCallable)
	void SetHp(float current, float max);

	UFUNCTION(BlueprintImplementableEvent)
	void SetMp(int32 current);

	UFUNCTION(BlueprintCallable)
	void SetStamina(float current, float max);

	UFUNCTION(BlueprintCallable)
	void SetStaminaEnable(bool enable);

	UFUNCTION(BlueprintCallable)
	void SetExp(float current, float max);

	UFUNCTION(BlueprintCallable)
	void SetNickname(const FString& nickname);

	UFUNCTION(BlueprintCallable)
	void SetLevel(int level);

	UFUNCTION(BlueprintImplementableEvent)
	void SetTarget(bool isTarget, FVector2D center, FVector2D size);
};
