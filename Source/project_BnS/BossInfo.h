// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BossInfo.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_BNS_API UBossInfo : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UProgressBar> HpBar;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UTextBlock> HpText;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UTextBlock>HpPercentText;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UTextBlock> LevelText;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UTextBlock> NameText;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UTextBlock> DIstanceText;

public:
	void OnChangeHp(float current, float max);
	void SetLevel(int32 level);
	void SetName(FString name);
	void SetDistance(float distance);

};
