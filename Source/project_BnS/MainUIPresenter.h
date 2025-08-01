// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


/**
 * 
 */
class PROJECT_BNS_API MainUIPresenter
{
public:
	MainUIPresenter(class UMainUi* ui, class AMyPlayer* player);
	~MainUIPresenter();

	// variable
	UMainUi* MainUI = nullptr;
	AMyPlayer* MyPlayer = nullptr;

	// Status
public:
	void OnHpChange(float current, float max) const;
	void OnMpChange(float current) const;
	void OnStanimaChange(float current, float max) const;
	void OnBattleChange(bool battle) const;

	void OnNicknameChange(const FString& nickname) const;
	void OnLevelChange(int level) const;
	void OnExpChange(float current, float max) const;

	void OnTargetChange(bool isTarget, FVector2D center, FVector2D size) const;
};
