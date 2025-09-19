// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../CrowdControlType.h"
#include "CrowdControlDisplay.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_BNS_API UCrowdControlDisplay : public UUserWidget
{
	GENERATED_BODY()

public:
	UCrowdControlDisplay(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

private:
	enum ECrowdControlType CurrentType;

	TObjectPtr<UTexture2D> TextureNone = nullptr;
	TObjectPtr<UTexture2D> TextureDown= nullptr;
	TObjectPtr<UTexture2D> TextureStun = nullptr;
	TObjectPtr<UTexture2D> TextureImmune = nullptr;
	
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> Image = nullptr;

public:
	void SetType(ECrowdControlType type);
};
