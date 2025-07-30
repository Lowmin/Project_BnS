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
	void SetHp(int current, int max);

	UFUNCTION(BlueprintImplementableEvent)
	void SetMp(int current);

	UFUNCTION(BlueprintImplementableEvent)
	void SetStamina(int current, int max);

	UFUNCTION(BlueprintImplementableEvent)
	void SetExp(int current, int max);

	//UFUNCTION(BlueprintImplementableEvent)
	//void SetNickname(FString& nickname);

	UFUNCTION(BlueprintImplementableEvent)
	void SetLevel(int level);
};
