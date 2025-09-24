// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BuffSlot.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_BNS_API UBuffSlot : public UUserWidget
{
	GENERATED_BODY()

public:
	UBuffSlot(const FObjectInitializer& ObjectInitializer);

private:
	UPROPERTY()
	TSubclassOf<class UBuffIcon> BuffIconClass = nullptr;
	
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UHorizontalBox> Root = nullptr;

	TArray<TObjectPtr<class UBuffIcon>> BuffIcons;

public:
	void AddBuff(const struct FBuffData& data);
	void RemoveBuff(const struct FBuffData& data);
};
