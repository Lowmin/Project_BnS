// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SoulShieldSlot.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_BNS_API USoulShieldSlot : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY()
	TObjectPtr<UTexture2D> Texture = nullptr;

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> SoulShield0 = nullptr;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> SoulShield1 = nullptr;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> SoulShield2 = nullptr;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> SoulShield3 = nullptr;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> SoulShield4 = nullptr;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> SoulShield5 = nullptr;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> SoulShield6 = nullptr;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> SoulShield7 = nullptr;

private:
	void SetTexture(class UImage* image, const class UItem* data);

public:
	void SetInfo(int32 idx, const class UItem* data);
};
