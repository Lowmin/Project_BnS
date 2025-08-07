// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "MainUIPresenter.generated.h"

/**
 * 
 */
 UCLASS()
class PROJECT_BNS_API AMainUIPresenter : public AActor
{
	GENERATED_BODY()

public:
	void SetMainUI(class UMainUi* ui);
 	void SetMyPlayer(class AMyPlayer* player);

 private:
 	void Bind(class UStatComponent* stat);
 	
	// variable
	UMainUi* MainUI = nullptr;
	AMyPlayer* MyPlayer = nullptr;

	// Status
public:
 	UFUNCTION()
	void OnHpChange(float current, float max) const;
 	UFUNCTION()
	void OnMpChange(int32 current) const;
 	UFUNCTION()
	void OnStanimaChange(float current, float max) const;
	void OnBattleChange(bool battle) const;

	void OnNicknameChange(const FString& nickname) const;
 	UFUNCTION()
	void OnLevelChange(int level) const;
 	UFUNCTION()
	void OnExpChange(float current, float max) const;

	void OnTargetChange(bool isTarget, FVector2D center, FVector2D size) const;
};
