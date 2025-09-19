// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../CrowdControlType.h"
#include "BossInfo.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_BNS_API UBossInfo : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

private:
	TArray<class UCrowdControlDisplay*> ImgCCList;
	
protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UProgressBar> HpBar = nullptr;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UTextBlock> HpText = nullptr;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UTextBlock>HpPercentText = nullptr;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UTextBlock> LevelText = nullptr;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UTextBlock> NameText = nullptr;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UHorizontalBox> CrowdControlRoot = nullptr;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UTextBlock> DIstanceText = nullptr;

public:
	void SetInfo(float curHp, float maxHp, int32 level, FString name, int32 activateCCCount, float distance);
	void OnChangeHp(float current, float max);
	void SetLevel(int32 level);
	void SetName(FString name);
	void SetMaxCrowdControlCount(int32 activateCCCount);
	void SetBossCCInfo(ECrowdControlType type, int32 count);
	void SetDistance(float distance);

};
