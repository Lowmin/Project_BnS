// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Popup.h"
#include "CharacterInfoPopup.generated.h"

class UTextBlock;

/**
 * 
 */

UCLASS()
class PROJECT_BNS_API UCharacterInfoPopup : public UPopup
{
	GENERATED_BODY()
	
protected:
	virtual void SetVisiblePopup(bool isVisible) override;

private:
	void UpdateCharacterInfo();

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_CharacterName;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_Level;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_HP;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_ATK;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_DEF;
};
