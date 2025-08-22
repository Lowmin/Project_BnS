// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MpIcon.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_BNS_API UMpIcon : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UImage> Icon;

public:
	void SetIconVisible(bool visible);
};
