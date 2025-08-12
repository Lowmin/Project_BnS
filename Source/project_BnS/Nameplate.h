// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Nameplate.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_BNS_API UNameplate : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UTextBlock> NameText = nullptr;

public:
	void SetNameplate(const FString name);
};
