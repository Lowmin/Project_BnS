// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HpBar.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_BNS_API UHpBar : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UProgressBar> HpBar;

public:
	void OnChangeHp(float current, float max);
};
