// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainUi.h"
#include "LevelUpWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_BNS_API ULevelUpWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetLevelUpText(int32 Level);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_LevelUpText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_Level;
};
